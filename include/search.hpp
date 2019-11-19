#ifndef _REPLICATOR_GAME_SEARCH_H_
#define _REPLICATOR_GAME_SEARCH_H_

#include "components.hpp"
#include "city.hpp"

std::vector<Destination> search( 
        const Position& initial_pos,
        const Place& final_destination,
        const std::vector<Place>& places
);

#endif // _REPLICATOR_GAME_SEARCH_H_
