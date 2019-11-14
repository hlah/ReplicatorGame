#include "city.hpp"

#include "replicator/transform.hpp"
#include "replicator/hierarchy.hpp"
#include "replicator/models.hpp"
#include "replicator/deepcopy.hpp"

#include <random>
#include <chrono>

#include "spdlog/spdlog.h"

struct Rect {
    int x;
    int z;
    unsigned int w;
    unsigned int l;
};

std::vector<Rect> divide_block( 
        std::default_random_engine& rng, 
        Rect block, 
        unsigned int max_size, 
        unsigned int max_w_divs = 3, 
        unsigned int max_l_divs = 3, 
        unsigned int road_size=4 ) 
{
    spdlog::debug("Dividing block: {} {} {} {}", block.x, block.z, block.w, block.l);
    std::vector<Rect> divisions;
    unsigned int w_divs = 1 + rng() % max_w_divs;
    unsigned int l_divs = 1 + rng() % max_l_divs;

    spdlog::debug("divisions: {} {}", w_divs, l_divs);

    auto pos_x = block.x;
    auto block_w = (block.w - road_size*(w_divs-1)) / w_divs;
    for( unsigned int i=0; i<w_divs; i++ ) {
        auto pos_z = block.z;
        auto block_l = (block.l - road_size*(l_divs-1)) / l_divs;
        if( i == w_divs-1 ) {
            block_w = block.w - (pos_x-block.x);
        }

        for( unsigned int j=0; j<l_divs; j++ ) {
            if( j == l_divs-1 ) {
                block_l = block.l - (pos_z-block.z);
            }

            Rect new_block{ pos_x, pos_z, block_w, block_l };
            if( new_block.w <= max_size && new_block.l <= max_size ) {
                divisions.push_back( new_block );
            } else {
                auto sub_divisions = divide_block( 
                        rng, new_block, max_size,
                        new_block.w <= max_size ? 1 : max_w_divs,
                        new_block.l <= max_size ? 1 : max_l_divs,
                        road_size
                );
                divisions.insert( divisions.end(), sub_divisions.begin(), sub_divisions.end() );
            }

            pos_z += road_size + block_l;
        }

        pos_x += road_size + block_w;
    }

    return divisions;
};

void make_city( 
        entt::registry& registry ,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::vector<Building>& buildings,
        Mesh mesh_rect,
        unsigned int width,
        unsigned int length,
        std::vector<DivisionLevel> divison_levels
) {
    auto rng = std::default_random_engine{ (unsigned) std::chrono::system_clock::now().time_since_epoch().count() };

    std::vector<Rect> blocks;
    blocks.push_back( { -(int)width/2, -(int)length/2, width, length } );
    for( const auto& division_level : divison_levels  ) {
        std::vector<Rect> new_blocks;
        for( const auto& block : blocks ) {
            auto sub_blocks = divide_block( rng, block, division_level.max_size, division_level.divs, division_level.divs, division_level.road_size );
            new_blocks.insert( new_blocks.end(), sub_blocks.begin(), sub_blocks.end() );
        }
        blocks = new_blocks;
    }

    for( auto& block : blocks ) {
        // block marker
        auto block_marker = registry.create();
        registry.assign<Model>( block_marker, mesh_rect, program_handle );
        registry.assign<Transform>( block_marker, Transform{}.translate(  (float)(block.x)+(float)block.w/2.f, 0.01, (float)(block.z)+(float)block.l/2.f).scale( (float)block.w/2.0, 1.0, (float)block.l/2.0 ) );
        registry.assign<Material>( 
                block_marker, 
                glm::vec3{0.0, 0.2, 0.0}, 
                glm::vec3{0.1, 0.5, 0.1}  
        );
        registry.assign<Hierarchy>( block_marker );

        // place buildings
        const auto& building_to_place = buildings[rng()%buildings.size()];
        auto building = deepcopy( registry, building_to_place.prefab );
        auto building_transform = registry.get<Transform>( building );
        building_transform.translate_global(  (float)(block.x)+(float)block.w/2.f, 0.0, (float)(block.z)+(float)block.l/2.f );
        registry.replace<Transform>( building, building_transform );
    }

    /// Create Terrain
    /*
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
    */
    spdlog::debug("Done!");
}
