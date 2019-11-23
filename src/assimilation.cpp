#include "assimilation.hpp"

#include "replicator/material.hpp" 
#include "replicator/hierarchy.hpp" 

#include "spdlog/spdlog.h"

void Assimilated::on_construct(entt::entity entity, entt::registry& registry, const Assimilated& assimilated) {
    Material assimiated_material{
            glm::vec3{0.1, 0.1, 0.0}, 
            glm::vec3{0.3, 0.7, 0.9},  
            glm::vec3{0.5, 0.5, 0.5},  
            50.0
    };

    const auto& hierarchy = registry.get<Hierarchy>( entity );
    auto next_children = hierarchy.first();
    while( next_children != entt::null ) {
        registry.assign_or_replace<Material>( next_children, assimiated_material );
        next_children = registry.get<Hierarchy>( next_children ).next();
    }


}
