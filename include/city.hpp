#ifndef _REPLICATOR_GAME_TERRAIN_H_
#define _REPLICATOR_GAME_TERRAIN_H_

#include "entt/entt.hpp"
#include "replicator/shaders.hpp"
#include "replicator/mesh.hpp"

struct Building {
    Mesh mesh;
    unsigned int width;
    unsigned int length;
    Building( Mesh mesh, unsigned int width, unsigned int length ) :
        mesh{mesh},
        width{width},
        length{length}
    { }
};

void make_city( 
        entt::registry& registry ,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::vector<Building>& buildings,
        Mesh mesh_rect,
        unsigned int width,
        unsigned int length
);

#endif // _REPLICATOR_GAME_TERRAIN_H_
