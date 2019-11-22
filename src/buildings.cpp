#include "buildings.hpp"

#include "replicator/model_loader.hpp"
#include "replicator/transform.hpp"

#include "spdlog/spdlog.h"

std::vector<Building> get_buildings( entt::registry& registry, entt::resource_handle<ShaderProgram> program_handle ) {
    std::vector<Building> buildings;

    buildings.push_back( get_building( registry, program_handle, "../models/buildings/house1/House N220518.3DS", 0.08 ) );
    //buildings.push_back( get_building( registry, program_handle, "../models/buildings/house2/House N210818.3ds", 0.08 ) );
    buildings.push_back( get_building( registry, program_handle, "../models/buildings/house4/House N180817.3DS", 0.0009 ) );
    buildings.push_back( get_building( registry, program_handle, "../models/buildings/house5/House 1 N210717.3DS", 1.0 ) );
    buildings.push_back( get_building( registry, program_handle, "../models/buildings/house6/House N250817.3DS", 0.0008 ) );
    return buildings;
}

Building get_building(
        entt::registry& registry,
        entt::resource_handle<ShaderProgram> program_handle,
        const std::string& model_path,
        float scalling_factor
) {

    ModelLoader model_loader{ program_handle };
    auto house = model_loader.load_model( registry, model_path );
    auto house_bounding_box = model_loader.bounding_box();
    auto house_transform = registry.get<Transform>( house );
    house_transform.scale( scalling_factor );
    house_transform.translate_global( 0.0, -scalling_factor*house_bounding_box.min().y, 0.0 );
    registry.replace<Transform>( house, house_transform );

    house_bounding_box.scale( scalling_factor );

    return Building{ 
            house, 
            house_bounding_box.width(),
            house_bounding_box.length(),
            house_bounding_box.min().x,
            house_bounding_box.min().z
    };
}
