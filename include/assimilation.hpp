#ifndef _REPLICATOR_GAME_ASSIMIALTION_H_
#define _REPLICATOR_GAME_ASSIMIALTION_H_

#include "entt/entt.hpp"

class Assimilated{
    public:
        // update components when selected component is added
        static void on_construct(entt::entity entity, entt::registry& registry, const Assimilated& assimilated);
};

void assimilation_system( entt::registry& registry );


#endif // _REPLICATOR_GAME_ASSIMIALTION_H_
