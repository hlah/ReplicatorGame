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
    Destination( glm::vec3 value = glm::vec3{0.0} ) : value{value} {}
};

struct Wait {
    float time;
};

#endif // _REPLICATOR_GAME_COMPONENTS__
