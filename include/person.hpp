#ifndef _REPLICATOR_GAME_PERSON_H_
#define _REPLICATOR_GAME_PERSON_H_

#include "entt/entt.hpp"
#include "replicator/mesh.hpp"
#include "replicator/shaders.hpp"
#include "city.hpp"

entt::entity new_person(
        entt::registry& registry,
        std::default_random_engine& rng,
        const std::vector<Place> places,
        const entt::resource_handle<ShaderProgram> program_handle,
        const Mesh& mesh_cylinder, 
        const Mesh& mesh_sphere
);

#endif // _REPLICATOR_GAME_PERSON_H_
