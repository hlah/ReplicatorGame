#include "city.hpp"

#include "replicator/transform.hpp"
#include "replicator/hierarchy.hpp"
#include "replicator/models.hpp"

#include <random>
#include <chrono>

#include "spdlog/spdlog.h"

struct CityBlock {
    int x;
    int z;
    unsigned int w;
    unsigned int l;
};

void divide_city_block( 
        std::default_random_engine& rng,
        std::vector<CityBlock>& blocks, 
        CityBlock city_block, 
        unsigned int min_block_size = 10, 
        unsigned int road_size = 2 
) {
    unsigned int w_divs = 1 + rng() % (city_block.w / (min_block_size+road_size) );
    unsigned int l_divs = 1 + rng() % (city_block.l / (min_block_size+road_size) );

    int posx = city_block.x;
    unsigned int blockw = city_block.w / w_divs;
    unsigned int blockl = city_block.l / l_divs;
    if( w_divs == 1 && l_divs == 1 ) {
        blocks.push_back( city_block );
    } else {
        for( unsigned int w_div=0; w_div<w_divs; w_div++ ) {
            int posz = city_block.z;
            for( unsigned int l_div=0; l_div<l_divs; l_div++ ) {
                divide_city_block( rng, blocks, { posx, posz, blockw, blockl }, min_block_size, road_size );
                posz += blockl + road_size;
            }
            posx += blockw + road_size;
        }
    }
}

void make_city( 
        entt::registry& registry ,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::vector<Building>& buildings,
        Mesh mesh_rect,
        unsigned int width,
        unsigned int length
) {
    auto rng = std::default_random_engine{ (unsigned) std::chrono::system_clock::now().time_since_epoch().count() };

    std::vector<CityBlock> city_blocks;
    divide_city_block( rng, city_blocks, { -(int)width / 2, -(int)length / 2, width, length  } );
    for( const auto& block : city_blocks ) {
        spdlog::debug(
            "position: {} {}, size: {} {}",
            block.x, block.z,
            block.w, block.l
        );
        // place buildings
        const auto& building_to_place = buildings[rng()%buildings.size()];
        auto building = registry.create();
        registry.assign<Hierarchy>( building );
        registry.assign<Model>( building, building_to_place.mesh, program_handle );
        registry.assign<Transform>( building, Transform{}.translate( (float)(block.x)+(float)block.w/2.f, 0.0, (float)(block.z)+(float)block.l/2.f ) );
        registry.assign<Material>( 
                building, 
                glm::vec3{0.0, 0.0, 0.0}, 
                glm::vec3{0.5, 0.5, 0.5},  
                glm::vec3{0.1, 0.1, 0.1},  
                2.0
        );
    }

    /// Create Terrain
    spdlog::debug("Creating terrain...");
    for( int i = -(int)width/4; i<(int)(width+1)/4; i++ ) {
        for( int j = -(int)length/4; j<(int)(length+1)/4; j++ ) {
            auto terrain = registry.create();
            registry.assign<Transform>( terrain, Transform{}.translate( 2*i, 0.0, 2*j ) );
            registry.assign<Hierarchy>( terrain );
            registry.assign<Model>( terrain, mesh_rect, program_handle );
            if( (i+j)%2 == 0 ) {
                registry.assign<Material>( 
                        terrain, 
                        glm::vec3{0.0, 0.0, 0.0}, 
                        glm::vec3{0.5, 0.5, 0.5},  
                        glm::vec3{0.2, 0.2, 0.2},  
                        20.0
                );
            } else {
                registry.assign<Material>( 
                        terrain, 
                        glm::vec3{0.0, 0.0, 0.0}, 
                        glm::vec3{0.8, 0.8, 0.8},  
                        glm::vec3{0.1, 0.1, 0.1},  
                        20.0
                );
            }
        }
    }
    spdlog::debug("Done!");
}
