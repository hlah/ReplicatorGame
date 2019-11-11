#include "person.hpp"

#include "replicator/hierarchy.hpp"
#include "replicator/transform.hpp"
#include "replicator/material.hpp"
#include "replicator/models.hpp"


entt::entity new_person(
        entt::registry& registry,
        const entt::resource_handle<ShaderProgram> program_handle,
        const Mesh& mesh_cylinder, 
        const Mesh& mesh_sphere
) {
    auto person = registry.create();
    registry.assign<Hierarchy>( person );
    registry.assign<Transform>( person );

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
