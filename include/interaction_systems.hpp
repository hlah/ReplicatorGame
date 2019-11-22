#ifndef _REPLICATOR_GAME_INTERACTION_SYSTEMS_H_
#define _REPLICATOR_GAME_INTERACTION_SYSTEMS_H_

#include "entt/entt.hpp"

struct SelectedHighlight {
    entt::entity prefab;
};

class Selected {
    public:
        Selected() : _highlight{ entt::null } {}

        // update components when selected component is added
        static void on_construct(entt::entity entity, entt::registry& registry, Selected& selected);

        // update components when selected component is removed
        static void on_destroy(entt::entity entity, entt::registry& registry);
    private:
        entt::entity _highlight;
};

// Update selection
void selection_system( entt::registry& registry );


#endif // _REPLICATOR_GAME_INTERACTION_SYSTEMS_H_
