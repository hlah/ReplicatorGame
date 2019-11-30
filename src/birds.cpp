#include "birds.hpp"

#include "components.hpp"
#include "bezier.hpp"

#include "replicator/transform.hpp"
#include "replicator/model_loader.hpp"
#include "replicator/deepcopy.hpp"

#include "spdlog/spdlog.h"

entt::entity generate_bird_prefab( entt::registry& registry, const entt::resource_handle<ShaderProgram> program_handle ) {
    ModelLoader model_loader{ program_handle };
    auto bird_prefab = model_loader.load_model( registry, "../models/animals/seagull.3ds" );
    auto bird_transform = registry.get<Transform>( bird_prefab );
    bird_transform.scale( 0.01 );
    bird_transform.translate_global( 0.0, 30.0, 0.0 );
    registry.replace<Transform>( bird_prefab, bird_transform );

    return bird_prefab;
}


entt::entity random_place_bird(
        entt::registry& registry,
        std::default_random_engine& rng,
        entt::entity bird_prefab
) {
    auto bird = deepcopy( registry, bird_prefab );

    std::vector<glm::vec3> points;
    std::vector<glm::vec3> tangents;
    for( int i=0; i<5; i++ ) {
        float x = -50 + (int)rng()%100;
        float z = -50 + (int)rng()%100;
        float y = 30 + (int)rng()%20;
        points.emplace_back( x, y, z );

        float tx = (rng()%100)/100.0;
        float ty = (rng()%100)/100.0;
        float tz = (rng()%100)/100.0;
        tangents.emplace_back( 10.f * glm::normalize( glm::vec3{ tx, ty, tz } ) );
    }

    Curve curve;
    for( int i=0; i<(int)points.size(); i++ ) {
        curve.add_segment( 
                points[i], 
                points[i] + tangents[i], 
                points[(i+1) % points.size()] - tangents[(i+1) % tangents.size()], 
                points[(i+1) % points.size()]
        );
    }

    registry.assign<Curve>( bird, curve );
    registry.assign<Position>( bird, glm::vec3{points[0].x, points[0].y, points[0].z} );
    registry.assign<Bird>( bird, 0.f );

    return bird;
}
