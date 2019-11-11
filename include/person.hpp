#ifndef _REPLICATOR_GAME_PERSON_H_
#define _REPLICATOR_GAME_PERSON_H_

#include "entt/entt.hpp"
#include "replicator/mesh.hpp"
#include "replicator/shaders.hpp"

entt::entity new_person(
        entt::registry& registry,
        const entt::resource_handle<ShaderProgram> program_handle,
        const Mesh& mesh_cylinder, 
        const Mesh& mesh_sphere
);

#endif // _REPLICATOR_GAME_PERSON_H_
