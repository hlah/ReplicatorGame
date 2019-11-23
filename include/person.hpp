#ifndef _REPLICATOR_GAME_PERSON_H_
#define _REPLICATOR_GAME_PERSON_H_

#include "entt/entt.hpp"
#include "replicator/mesh.hpp"
#include "replicator/shaders.hpp"
#include "city.hpp"

struct Person{};


entt::entity generate_person_prefab( entt::registry& registry, const entt::resource_handle<ShaderProgram> program_handle );

entt::entity random_place_person(
        entt::registry& registry,
        std::default_random_engine& rng,
        const std::vector<Place> places,
        entt::entity person_prefab
);

entt::entity place_person(
        entt::registry& registry,
        entt::entity person_prefab,
        float x,
        float z
);



#endif // _REPLICATOR_GAME_PERSON_H_
