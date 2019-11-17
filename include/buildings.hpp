#ifndef _REPLICATOR_GAME_BUILDINGS_H_
#define _REPLICATOR_GAME_BUILDINGS_H_

#include "entt/entt.hpp"
#include "replicator/shaders.hpp"

struct Building {
    entt::entity prefab;
    float width;
    float length;
    float back;
    float left;
    Building( entt::entity prefab, float width, float length, float left, float back ) :
        prefab{prefab},
        width{width},
        length{length},
        back{back},
        left{left}
    { }
};

Building get_building(
        entt::registry& registry,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::string& model_path,
        float scalling_factor
);
std::vector<Building> get_buildings( entt::registry& registry, entt::resource_handle<ShaderProgram> program_handle );


#endif // _REPLICATOR_GAME_BUILDINGS_H_
