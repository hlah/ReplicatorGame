#ifndef _REPLICATOR_GAME_TERRAIN_H_
#define _REPLICATOR_GAME_TERRAIN_H_

#include "entt/entt.hpp"
#include "replicator/shaders.hpp"
#include "replicator/mesh.hpp"

#include "buildings.hpp"

#include <random>

struct DivisionLevel {
    unsigned int divs;
    unsigned int max_size;
    unsigned int road_size;
};

struct CityRect {
    int x;
    int z;
    unsigned int w;
    unsigned int l;
    inline bool inside( int vx, int vz ) const { return vx >= x && vx < x+(int)w && vz >= z && vz < z+(int)l; }
    inline bool operator!=( const CityRect& other ) const { 
        return x != other.x || z != other.z || w != other.w || l != other.l;
    }
    inline bool operator==( const CityRect& other ) const { 
        return !(*this != other);
    }
};

struct Place {
    float pos_x;
    float pos_z;
    CityRect rect;
    Place( float x, float z, const CityRect& rect ) : 
        pos_x{x}, pos_z{z}, rect{rect} {}
};

std::vector<Place> make_city( 
        entt::registry& registry ,
        std::default_random_engine& rng,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::vector<Building>& buildings,
        Mesh mesh_rect,
        unsigned int width,
        unsigned int length,
        std::vector<DivisionLevel> divison_levels
);

#endif // _REPLICATOR_GAME_TERRAIN_H_
