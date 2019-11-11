#ifndef _REPLICATOR_GAME_COMPONENTS_
#define _REPLICATOR_GAME_COMPONENTS_

#include "glm/vec3.hpp"

struct Position {
    glm::vec3 value{0.0};
};

struct Velocity {
    glm::vec3 value{0.0};
};

struct Destination {
    glm::vec3 value{0.0};
};

#endif // _REPLICATOR_GAME_COMPONENTS__
