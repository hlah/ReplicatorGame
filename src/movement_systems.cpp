#include "systems.hpp"

#include "replicator/transform.hpp"
#include "replicator/time.hpp"
#include "components.hpp"
#include "city.hpp"

#include <algorithm>
#include <random>

void position_system( entt::registry& registry ) {
    auto view = registry.view<Position, Transform>();
    view.each([&registry]( const auto entity, const auto& position, const auto& transform ){
            auto new_transform = transform;
            new_transform.set_translation( position.value );
            registry.replace<Transform>( entity, new_transform );
    });
}


void velocity_system( entt::registry& registry ) {
    auto view = registry.view<Velocity, Position>();
    auto dt = registry.ctx<DeltaTime>().value;
    view.each([dt]( const auto& velocity, auto& position ) {
            position.value += velocity.value * (float)dt;
    });
}

void destination_system( entt::registry& registry ) {
    auto view = registry.view<Position, Destination, Velocity>();
    view.each([]( const auto& position, const auto& destination, auto& velocity ) {
            auto displacement = destination.value - position.value;
            float distance = glm::length( displacement );
            velocity.value = glm::normalize(displacement) * 2.0f * std::min(1.0f, distance*10.f);
    });
}

void reallocation_system( entt::registry& registry ) {
    auto& rng = registry.ctx<std::default_random_engine>();
    const auto& places = registry.ctx<std::vector<Place>>();
    auto view = registry.view<Velocity>();
    view.each([&registry, &rng, &places]( const auto entity, const auto& velocity ) {
            if( glm::length( velocity.value ) < 0.01f ) {
                const auto& new_place = places[ rng() % places.size() ];
                Destination new_destination{ glm::vec3{ new_place.pos_x, 0.0, new_place.pos_z } };
                registry.assign_or_replace<Destination>( entity, new_destination );
            }
    });
}
