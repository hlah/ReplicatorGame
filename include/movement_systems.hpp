#ifndef _REPLICATOR_GAME_SYSTEMS_
#define _REPLICATOR_GAME_SYSTEMS_

#include "entt/entt.hpp"

// Update entity transform based on its positon
void position_system( entt::registry& registry );

// Update entity position based on its velocity
void velocity_system( entt::registry& registry );

// Update entity velocity based on its destination
void destination_system( entt::registry& registry );


#endif // _REPLICATOR_GAME_SYSTEMS_
