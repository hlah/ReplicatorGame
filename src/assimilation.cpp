#include "assimilation.hpp"

#include "entt/entity/fwd.hpp"
#include "entt/entity/utility.hpp"
#include "person.hpp"
#include "components.hpp"

#include "replicator/material.hpp" 
#include "replicator/hierarchy.hpp" 
#include "replicator/geometry/box.hpp"

#include "spdlog/spdlog.h"

#include <set>

void Assimilated::on_construct(entt::entity entity, entt::registry& registry, const Assimilated& assimilated) {
    Material assimiated_material{
            glm::vec3{0.0, 0.1, 0.3}, 
            glm::vec3{0.3, 0.5, 0.7},  
            glm::vec3{0.3, 0.5, 0.7},  
            20.0
    };

    const auto& hierarchy = registry.get<Hierarchy>( entity );
    auto next_children = hierarchy.first();
    while( next_children != entt::null ) {
        registry.assign_or_replace<Material>( next_children, assimiated_material );
        next_children = registry.get<Hierarchy>( next_children ).next();
    }

    registry.get<std::vector<Destination>>( entity ).clear();
}

void assimilation_system( entt::registry& registry ) {
    auto view = registry.view<const Box, const Transform, Person, Assimilated>();
    auto group = registry.group<const Box, Person>(entt::get<Transform>, entt::exclude<Assimilated>);
    std::set<entt::entity> converted;

    view.each([&converted, group](const auto& box, const auto& transform, auto person, auto assimilated){
            auto transformed_box = transform.global_matrix() * box;
            group.each([&converted, &transformed_box](auto entity, const auto& box2, auto person, const auto& transform2) {
                    if( transformed_box.intersects( transform2.global_matrix() * box2 ) ) {
                        converted.insert(entity);
                    }
            });
    });

    for( auto entity : converted ) {
        spdlog::debug("{} is now ASSIMILATED!", entity);
        registry.assign<Assimilated>( entity );
    }
}

