#include "city.hpp"

#include "replicator/transform.hpp"
#include "replicator/hierarchy.hpp"
#include "replicator/models.hpp"
#include "replicator/deepcopy.hpp"

#include <chrono>

#include "spdlog/spdlog.h"


std::vector<CityRect> divide_block( 
        std::default_random_engine& rng, 
        CityRect block, 
        unsigned int max_size, 
        unsigned int max_w_divs = 3, 
        unsigned int max_l_divs = 3, 
        unsigned int road_size=4 ) 
{
    //spdlog::debug("Dividing block: {} {} {} {}", block.x, block.z, block.w, block.l);
    std::vector<CityRect> divisions;
    unsigned int w_divs = 1 + rng() % max_w_divs;
    unsigned int l_divs = 1 + rng() % max_l_divs;

    //spdlog::debug("divisions: {} {}", w_divs, l_divs);

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

            CityRect new_block{ pos_x, pos_z, block_w, block_l };
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

std::vector<Place> make_city( 
        entt::registry& registry ,
        std::default_random_engine& rng,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::vector<Building>& buildings,
        Mesh mesh_rect,
        unsigned int width,
        unsigned int length,
        std::vector<DivisionLevel> divison_levels
) {
    std::vector<Place> places;

    std::vector<CityRect> blocks;
    blocks.push_back( { -(int)width/2, -(int)length/2, width, length } );
    for( const auto& division_level : divison_levels  ) {
        std::vector<CityRect> new_blocks;
        for( const auto& block : blocks ) {
            auto sub_blocks = divide_block( rng, block, division_level.max_size, division_level.divs, division_level.divs, division_level.road_size );
            new_blocks.insert( new_blocks.end(), sub_blocks.begin(), sub_blocks.end() );
        }
        blocks = new_blocks;
    }

    auto& texture_cache = registry.ctx<entt::resource_cache<Texture>>();
    auto grass_texture = texture_cache.load<TextureLoader>(
            "grass_texture"_hs,
            "../textures/grass.jpg"
    );

    for( auto& block : blocks ) {
        // block marker
        MeshBuilder grass_mb{};
        grass_mb.rect( glm::vec3{0.0}, glm::vec3{ 0.0, 0.0, -(float)block.l/2.0 }, glm::vec3{ (float)block.w/2.0, 0.0, 0.0 } );
        grass_mb.clear_texcoord();
        grass_mb.add_texcoord( { 0.0,  0.0} );
        grass_mb.add_texcoord( { block.w/3.0,  0.0} );
        grass_mb.add_texcoord( { block.w/3.0,  block.l/3.0} );
        grass_mb.add_texcoord( { 0.0,  block.l/3.0} );
        auto grass_mesh = grass_mb.build();

        auto block_marker = registry.create();
        registry.assign<Model>( block_marker, grass_mesh, program_handle );
        registry.assign<Transform>( block_marker, Transform{}.translate(  (float)(block.x)+(float)block.w/2.f, 0.01, (float)(block.z)+(float)block.l/2.f) );
        Material grass_material{
                glm::vec3{0.0, 0.2, 0.0}, 
                glm::vec3{0.05, 0.3, 0.05}  
        };
        grass_material.add_diffuse_texture( grass_texture );
        registry.assign<Material>( block_marker, grass_material );
        registry.assign<Hierarchy>( block_marker );


        // select building to place
        std::vector<size_t> elegible_buildings;
        for( size_t i=0; i<buildings.size(); i++ ) {
            if( buildings[i].width < block.w && buildings[i].length < block.l ) {
                elegible_buildings.push_back(i);
            }
        }
        const auto& building_to_place = buildings[elegible_buildings[rng()%elegible_buildings.size()]];

        // place buildings
        auto building = deepcopy( registry, building_to_place.prefab );
        auto building_transform = registry.get<Transform>( building );
        float building_pos_x = (float)(block.x)+(float)block.w/2.f-building_to_place.left-building_to_place.width/2.f; 
        float building_pos_z = (float)(block.z)+(float)block.l/2.f-building_to_place.back-building_to_place.length/2.f;
        building_transform.translate_global( building_pos_x, 0.0, building_pos_z );
        registry.replace<Transform>( building, building_transform );
        places.emplace_back( (float)(block.x)+(float)block.w/2.f, (float)(block.z)+(float)block.l/2.f, block );

    }


    // place terrain
    MeshBuilder terrain_mb{};
    terrain_mb.rect( glm::vec3{0.0}, glm::vec3{ 0.0, 0.0, -(float)length/2.0-10.0 }, glm::vec3{ (float)width/2.0+10.0, 0.0, 0.0 } );
    terrain_mb.clear_texcoord();
    terrain_mb.add_texcoord( { 0.0,  0.0} );
    terrain_mb.add_texcoord( { width+20.0,  0.0} );
    terrain_mb.add_texcoord( { width+20.0,  length+20.0} );
    terrain_mb.add_texcoord( { 0.0,  length+20.0} );
    auto terrain_mesh = terrain_mb.build();

    auto terrain = registry.create();
    registry.assign<Transform>( terrain );
    registry.assign<Hierarchy>( terrain );
    registry.assign<Model>( terrain, terrain_mesh, program_handle );

    auto terrain_texture = texture_cache.load<TextureLoader>(
            "terrain_texture"_hs,
            "../textures/sidewalk2.png"
    );
    Material terrain_material{
            glm::vec3{0.2, 0.2, 0.2}, 
            glm::vec3{0.0, 0.0, 0.0},  
            glm::vec3{0.5, 0.5, 0.5},  
            2.0
    };
    terrain_material.add_diffuse_texture( terrain_texture );
    registry.assign<Material>( terrain, terrain_material );

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
    return places;
}
