#include "interaction_systems.hpp"

#include "person.hpp"
#include "components.hpp"
#include "search.hpp"
#include "assimilation.hpp"

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

    std::optional<entt::entity> selected;
    float distance = -std::numeric_limits<float>::infinity();

    auto view = registry.view<Box, Transform, Person, Assimilated>();
    view.each([&ray, &distance, &selected](auto entity, const auto& box, const auto& transform, const auto& person, const auto& assimiliated ){
            auto transformed_box = transform.global_matrix() * box;
            auto intersection_test = ray.intersects( transformed_box );
            if( intersection_test && intersection_test->first > distance  ) {
                distance = intersection_test->first;
                selected = entity;
            } 
    });

    registry.reset<Selected>();
    if( selected ) {
        registry.assign<Selected>( *selected );
    } 
}

void command_system( entt::registry& registry ) {
    auto window = registry.ctx<WindowHandler>();
    auto ray = Ray::from_screen( registry, window->mouse_x(), window->mouse_y() );

    Plane plane{ glm::vec3{0.0}, glm::vec3{0.0, 1.0, 0.0} };
    auto intersection_test = ray.intersects( plane );
    if( intersection_test && intersection_test->first > 0.0 ) {
        auto view = registry.view<Person, Selected, Position, std::vector<Destination>>();
        view.each([&registry, &intersection_test](
                    const auto& person, 
                    const auto& selected, 
                    const auto& position, 
                    auto& destination) 
        {
                const auto& places = registry.ctx<std::vector<Place>>();
                Place final_dest{ intersection_test->second.x, intersection_test->second.z };
                destination = search( position, final_dest, places, true );
        });
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
