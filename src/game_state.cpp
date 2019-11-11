#include "game_state.hpp"

#include "replicator/window.hpp"
#include "replicator/mesh.hpp"
#include "replicator/models.hpp"
#include "replicator/transform.hpp"
#include "replicator/camera.hpp"
#include "replicator/hierarchy.hpp"
#include "replicator/material.hpp"
#include "replicator/matrix_op.hpp"

#include "person.hpp"
#include "components.hpp"
#include "systems.hpp"

State::Transition GameState::on_start( entt::registry& registry ) {
    auto& program_cache = registry.set<entt::resource_cache<ShaderProgram>>();
    auto& texture_cache = registry.set<entt::resource_cache<Texture>>();

    MeshBuilder mb_rect;
    mb_rect.rect( glm::vec3{0.0}, glm::vec3{0.0, 0.0, -1.0}, glm::vec3{1.0, 0.0, 0.0} );
    auto mesh_rect = mb_rect.build();

    MeshBuilder mb_cylinder;
    mb_cylinder.cylinder( glm::vec3{0.2, 0.0, 0.0}, glm::vec3{0.0, 0.5, 0.0}, 64, glm::vec3{0.0, 0.5, 0.0} );
    auto mesh_cylinder = mb_cylinder.build();

    MeshBuilder mb_sphere;
    mb_sphere.icosphere( 0.3, 3 );
    auto mesh_sphere = mb_sphere.build();

    auto program_handle = program_cache.load<ShaderProgramLoader>(
            "shader_program"_hs, 
            std::vector<std::string>{"../shaders/vertex_main.glsl"}, 
            std::vector<std::string>{
            "../shaders/fragment_main.glsl", 
            "../shaders/blinn_phong_shading.glsl", 
            "../shaders/lights.glsl"
            } 
    );

    for( int i = 0; i<100; i++ ) {
        auto person = new_person(registry, program_handle, mesh_cylinder, mesh_sphere);
        registry.assign<Position>( person );
        registry.assign<Velocity>( person );
    }

    /// Create Terrain
    for( int i = -10; i<=10; i++ ) {
        for( int j = -10; j<=10; j++ ) {
            auto terrain = registry.create();
            registry.assign<Transform>( terrain, Transform{}.translate( 2*i, 0.0, 2*j ) );
            registry.assign<Hierarchy>( terrain );
            registry.assign<Model>( terrain, mesh_rect, program_handle );
            if( (i+j)%2 == 0 ) {
                registry.assign<Material>( 
                        terrain, 
                        glm::vec3{0.0, 0.0, 0.0}, 
                        glm::vec3{0.5, 0.5, 0.5},  
                        glm::vec3{0.2, 0.2, 0.2},  
                        20.0
                );
            } else {
                registry.assign<Material>( 
                        terrain, 
                        glm::vec3{0.0, 0.0, 0.0}, 
                        glm::vec3{0.8, 0.8, 0.8},  
                        glm::vec3{0.1, 0.1, 0.1},  
                        20.0
                );
            }
        }
    }

    //// Create player with camera
    _player = registry.create();
    registry.assign<Transform>( _player, Transform{}.translate(0.0, 0.0, 5.0) );
    registry.assign<Hierarchy>( _player );

    _head = registry.create();
    registry.assign<Transform>( _head, Transform{}.translate(0.0, 2.0, 0.0) );
    registry.assign<Hierarchy>( _head, _player );
    registry.set<CurrentCamera>( _head );

    auto camera = registry.create();
    registry.assign<Camera>( camera, (float)M_PI/2.f, -0.1f, -50.0f );
    registry.assign<Transform>( camera );
    registry.assign<Hierarchy>( camera, _head );
    registry.set<CurrentCamera>( camera );

    //// Lights

    auto light = registry.create();
    registry.assign<LightColor>( light, glm::vec3{0.5, 0.5, 0.5} );
    registry.assign<DirectionalLight>( light );
    registry.assign<Transform>( light, Transform{}.rotate_y_global( (float)M_PI/2.f ).rotate_z_global( (float)M_PI/4.f ) );
    registry.assign<Hierarchy>( light );

    // Set previous mouse position
    auto window = registry.ctx<std::shared_ptr<Window>>();
    _new_mouse_x = _prev_mouse_x = window->mouse_x();
    _new_mouse_y = _prev_mouse_y = window->mouse_y();

    // Set mouse capture mode
    window->capture_mouse(true);

    return State::Transition::NONE;
}

State::Transition GameState::on_action( entt::registry& registry, const ActionEvent& action ) {
    if( action.name() == "Close" && action.type() == ActionEvent::Type::ON ) {
        return State::Transition::QUIT;
    }
    handle_action( action, "CameraMoveRight", _player_horizontal_v, _player_speed );
    handle_action( action, "CameraMoveLeft", _player_horizontal_v, -_player_speed );
    handle_action( action, "CameraMoveForward", _player_transversal_v, -_player_speed );
    handle_action( action, "CameraMoveBackward", _player_transversal_v, _player_speed );

    return State::Transition::NONE;
}

State::Transition GameState::on_mouse_move( entt::registry& registry, double mx, double my ) {
    _new_mouse_x = mx;
    _new_mouse_y = my;
    return State::Transition::NONE;
}

State::Transition GameState::update( entt::registry& registry ) {
    _head_x_rotation = -(_new_mouse_y-_prev_mouse_y)*_player_rotation_speed;
    _player_y_rotation = -(_new_mouse_x-_prev_mouse_x)*_player_rotation_speed;

    auto new_player_transform = registry.get<Transform>( _player );
    new_player_transform.translate(_player_horizontal_v, 0.0, _player_transversal_v);
    new_player_transform.rotate_y( _player_y_rotation );
    registry.replace<Transform>( _player, new_player_transform );

    auto new_head_transform = registry.get<Transform>( _head );
    new_head_transform.rotate_x( _head_x_rotation );
    registry.replace<Transform>( _head, new_head_transform );

    destination_system( registry );
    velocity_system( registry );
    reallocation_system( registry );
    position_system( registry );
    transform_system( registry );
    camera_system( registry );
    light_system( registry );
    model_system( registry );

    _prev_mouse_x = _new_mouse_x;
    _prev_mouse_y = _new_mouse_y;

    return State::Transition::NONE;
}

// Helper function for action that edit float values
void GameState::handle_action( ActionEvent action, const std::string& action_name, float& value_to_edit, float value ) {
    if( action.name() == action_name) {
        if ( action.type() == ActionEvent::Type::ON  ) {
            value_to_edit += value;
        }
        if ( action.type() == ActionEvent::Type::OFF  ) {
            value_to_edit -= value;
        }
    }
}


