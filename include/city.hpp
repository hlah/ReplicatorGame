#ifndef _REPLICATOR_GAME_TERRAIN_H_
#define _REPLICATOR_GAME_TERRAIN_H_

#include "entt/entt.hpp"
#include "replicator/shaders.hpp"
#include "replicator/mesh.hpp"

#include "buildings.hpp"

struct DivisionLevel {
    unsigned int divs;
    unsigned int max_size;
    unsigned int road_size;
};

void make_city( 
        entt::registry& registry ,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::vector<Building>& buildings,
        Mesh mesh_rect,
        unsigned int width,
        unsigned int length,
        std::vector<DivisionLevel> divison_levels
);

#endif // _REPLICATOR_GAME_TERRAIN_H_
