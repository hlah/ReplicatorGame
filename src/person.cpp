#include "person.hpp"

#include "replicator/hierarchy.hpp"
#include "replicator/transform.hpp"
#include "replicator/material.hpp"
#include "replicator/models.hpp"
#include "replicator/deepcopy.hpp"

#include "components.hpp"

#include "spdlog/spdlog.h"

entt::entity generate_person_prefab( entt::registry& registry, const entt::resource_handle<ShaderProgram> program_handle ) {

    MeshBuilder mb_cylinder;
    mb_cylinder.cylinder( glm::vec3{0.2, 0.0, 0.0}, glm::vec3{0.0, 0.5, 0.0}, 64, glm::vec3{0.0, 0.5, 0.0} );
    auto mesh_cylinder = mb_cylinder.build();

    MeshBuilder mb_sphere;
    mb_sphere.icosphere( 0.3, 3 );
    auto mesh_sphere = mb_sphere.build();

    auto person = registry.create();

    registry.assign<Hierarchy>( person );

    Material person_material{
            glm::vec3{0.3, 0.3, 0.0}, 
            glm::vec3{0.9, 0.9, 0.3},  
    };

    // create body
    auto body = registry.create();
    registry.assign<Model>( body, mesh_cylinder, program_handle );
    registry.assign<Material>( body, person_material );
    registry.assign<Hierarchy>( body, person );
    registry.assign<Transform>( body );

    // create head
    auto head = registry.create();
    registry.assign<Model>( head, mesh_sphere, program_handle );
    registry.assign<Material>( head, person_material );
    registry.assign<Hierarchy>( head, person );
    auto head_transform = Transform{}.translate( 0.0, 1.3, 0.0 );
    registry.assign<Transform>( head, head_transform );

    auto cylinder_box = mb_cylinder.bounding_box( glm::mat4{1.0} ); 
    auto sphere_box = mb_sphere.bounding_box( head_transform.local_matrix() );
    auto box = cylinder_box + sphere_box;
    registry.assign<Box>( person, box );

    return person;
}

entt::entity random_place_person(
        entt::registry& registry,
        std::default_random_engine& rng,
        const std::vector<Place> places,
        entt::entity person_prefab
) {
    // select place
    auto& place = places[rng() % places.size()];

    auto person = deepcopy( registry, person_prefab );

    registry.assign<Transform>( person );
    registry.assign<Position>( person, glm::vec3{place.pos_x, 0.0, place.pos_z} );
    registry.assign<Velocity>( person );
    registry.assign<Person>( person );
    registry.assign<std::vector<Destination>>( person );

    return person;
}

entt::entity place_person(
        entt::registry& registry,
        entt::entity person_prefab,
        float x,
        float z
        ) {
    auto person = deepcopy( registry, person_prefab );

    registry.assign<Transform>( person );
    registry.assign<Position>( person, glm::vec3{x, 0.0, z} );
    registry.assign<Velocity>( person );
    registry.assign<Person>( person );
    registry.assign<std::vector<Destination>>( person );

    return person;
}
