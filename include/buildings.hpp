#ifndef _REPLICATOR_GAME_BUILDINGS_H_
#define _REPLICATOR_GAME_BUILDINGS_H_

#include "entt/entt.hpp"
#include "replicator/shaders.hpp"

struct Building {
    entt::entity prefab;
    unsigned int width;
    unsigned int length;
    Building( entt::entity prefab, unsigned int width, unsigned int length ) :
        prefab{prefab},
        width{width},
        length{length}
    { }
};

std::vector<Building> get_buildings( entt::registry& registry, entt::resource_handle<ShaderProgram> program_handle );


#endif // _REPLICATOR_GAME_BUILDINGS_H_
