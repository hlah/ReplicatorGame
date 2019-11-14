#include "buildings.hpp"

#include "replicator/model_loader.hpp"
#include "replicator/transform.hpp"

std::vector<Building> get_buildings( entt::registry& registry, entt::resource_handle<ShaderProgram> program_handle ) {
    std::vector<Building> buildings;

    auto house = load_model(
            registry,
            "../models/buildings/house4/House N180817.3DS",
            program_handle
    );
    auto house_transform = registry.get<Transform>( house );
    house_transform.scale( 0.0008, 0.0008, 0.0008 );
    registry.replace<Transform>( house, house_transform );

    buildings.emplace_back( house, 5u, 5u );

    return buildings;
}
