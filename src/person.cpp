#include "person.hpp"

#include "replicator/hierarchy.hpp"
#include "replicator/transform.hpp"
#include "replicator/material.hpp"
#include "replicator/models.hpp"

#include "components.hpp"

entt::entity new_person(
        entt::registry& registry,
        std::default_random_engine& rng,
        const std::vector<Place> places,
        const entt::resource_handle<ShaderProgram> program_handle,
        const Mesh& mesh_cylinder, 
        const Mesh& mesh_sphere
) {
    // select place
    auto& place = places[rng() % places.size()];


    auto person = registry.create();
    registry.assign<Hierarchy>( person );
    registry.assign<Transform>( person );
    registry.assign<Position>( person, glm::vec3{place.pos_x, 0.0, place.pos_z} );
    registry.assign<Velocity>( person );

    // create body
    auto body = registry.create();
    registry.assign<Model>( body, mesh_cylinder, program_handle );
    registry.assign<Material>( 
            body, 
            glm::vec3{0.1, 0.1, 0.0}, 
            glm::vec3{0.9, 0.9, 0.3},  
            glm::vec3{0.5, 0.5, 0.5},  
            50.0
    );
    registry.assign<Hierarchy>( body, person );
    registry.assign<Transform>( body );

    // create head
    auto head = registry.create();
    registry.assign<Model>( head, mesh_sphere, program_handle );
    registry.assign<Material>( 
            head, 
            glm::vec3{0.1, 0.1, 0.0}, 
            glm::vec3{0.9, 0.9, 0.3},  
            glm::vec3{0.5, 0.5, 0.5},  
            50.0
    );
    registry.assign<Hierarchy>( head, person );
    registry.assign<Transform>( head, Transform{}.translate( 0.0, 1.3, 0.0 ) );

    return person;
}
