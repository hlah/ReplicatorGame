#include "systems.hpp"

#include "replicator/transform.hpp"
#include "replicator/time.hpp"
#include "components.hpp"

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
    static std::default_random_engine rng;
    static std::uniform_real_distribution<float> dist{-20.0, 20.0};
    auto view = registry.view<Velocity>();
    view.each([&registry]( const auto entity, const auto& velocity ) {
            if( glm::length( velocity.value ) < 0.01f ) {
                Destination new_destination{ glm::vec3{ dist(rng), 0.0, dist(rng) } };
                registry.assign_or_replace<Destination>( entity, new_destination );
            }
    });
}
