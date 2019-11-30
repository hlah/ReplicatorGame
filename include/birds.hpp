#ifndef _REPLICATOR_GAME_BIRDS_H_
#define _REPLICATOR_GAME_BIRDS_H_

#include "entt/entt.hpp"
#include "replicator/shaders.hpp"

#include <random>

struct Bird{
    float t;
};

entt::entity generate_bird_prefab( entt::registry& registry, const entt::resource_handle<ShaderProgram> program_handle );

entt::entity random_place_bird(
        entt::registry& registry,
        std::default_random_engine& rng,
        entt::entity bird_prefab
);

#endif // _REPLICATOR_GAME_BIRDS_H_
