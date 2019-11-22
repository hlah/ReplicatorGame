#include "interaction_systems.hpp"

#include "person.hpp"

#include "replicator/geometry/ray.hpp"
#include "replicator/geometry/box.hpp"
#include "replicator/window.hpp"
#include "replicator/transform.hpp"
#include "replicator/deepcopy.hpp"
#include "replicator/models.hpp"

#include "spdlog/spdlog.h"
#include "entt/entt.hpp"


void selection_system( entt::registry& registry ) {
    auto window = registry.ctx<WindowHandler>();
    auto ray = Ray::from_screen( registry, window->mouse_x(), window->mouse_y() );

    /*
       Plane plane{ glm::vec3{0.0}, glm::vec3{0.0, 1.0, 0.0} };
       auto intersection_test = ray.intersects( plane );
       if( intersection_test ) {
       spdlog::debug("Intersection distance: {}; Position: {} {}", intersection_test->first, intersection_test->second.x, intersection_test->second.z);
       } else {
       spdlog::debug("No Intersection");
       }
       */

    std::optional<entt::entity> selected;
    float distance = -std::numeric_limits<float>::infinity();

    auto view = registry.view<Box, Transform, Person>();
    view.each([&ray, &distance, &selected](auto entity, const auto& box, const auto& transform, const auto& person){
            auto transformed_box = transform.global_matrix() * box;
            auto intersection_test = ray.intersects( transformed_box );
            if( intersection_test && intersection_test->first > distance  ) {
                distance = intersection_test->first;
                selected = entity;
            } 
    });

    registry.reset<Selected>();
    if( selected ) {
        spdlog::debug("Selected {}", (int)*selected);
        registry.assign<Selected>( *selected );
    } 
}

void Selected::on_construct(entt::entity entity, entt::registry& registry, Selected& selected) {
    auto highlight_prefab_ptr = registry.try_ctx<SelectedHighlight>();
    if( highlight_prefab_ptr != nullptr ) {
        selected._highlight = deepcopy( registry, highlight_prefab_ptr->prefab, entity );
        registry.reset<Hidden>( selected._highlight );
    }
}

void Selected::on_destroy(entt::entity entity, entt::registry& registry) {
    auto highlight_entity = registry.get<Selected>( entity )._highlight;
    if( registry.valid( highlight_entity )) {
        deepdelete( registry, highlight_entity );
    }
}
