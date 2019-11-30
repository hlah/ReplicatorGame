#include "movement_systems.hpp"

#include "replicator/transform.hpp"
#include "replicator/time.hpp"
#include "components.hpp"
#include "city.hpp"
#include "search.hpp"
#include "assimilation.hpp"
#include "birds.hpp"
#include "bezier.hpp"

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
    auto view = registry.view<Position, std::vector<Destination>, Velocity>();
    view.each([&registry]( auto entity, const auto& position, auto& destinations, auto& velocity ) {
            // add new destination
            if( destinations.size() == 0 && !registry.has<Assimilated>( entity ) ) {
                auto& rng = registry.ctx<std::default_random_engine>();
                const auto& places = registry.ctx<std::vector<Place>>();
                const auto& new_place = places[ rng() % places.size() ];
                destinations = search( position, new_place, places );
            }

            if( destinations.size() != 0 ) {
                auto& next_destination = destinations.back();
                auto displacement = next_destination.value - position.value;
                float distance = glm::length( displacement );
                if( distance < 0.5f ) {
                    destinations.pop_back();
                } else {
                    velocity.value = glm::normalize(displacement) * 2.0f * std::min(1.0f, distance*10.f);
                }
            } else {
                velocity.value = glm::vec3{0.0};
            }
    });
}

void bird_system( entt::registry& registry ) {
    auto view = registry.view<Position, const Curve, Bird>();
    auto dt = registry.ctx<DeltaTime>().value;
    view.each([dt]( auto& position, const auto& curve, auto& bird ){
            bird.t += (float)dt * 0.3;
            if( bird.t >= curve.tmax() ) {
                bird.t -= curve.tmax();
            }
            position.value = curve.get_point( bird.t );
    });
}
