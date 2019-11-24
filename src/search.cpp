#include "search.hpp"

#include <queue>
#include <map>
#include <optional>

#include "spdlog/spdlog.h"

#define SEARCH_SQRT2 1.5

typedef glm::ivec2 State;

struct SearchNode {
    State state;
    std::optional<State> prev_state;
    float g;
    float h;
    SearchNode( const State& state, const std::optional<State>& prev_state = {}, float g=0.0, float h=0.0 )  :
        state{state}, prev_state{prev_state}, g{g}, h{h}{}
    inline unsigned int f() const { return g+h; }
    inline bool operator<( const SearchNode& other ) const {
        return f() > other.f();
    }
};

struct StateHash {
    size_t operator()(const glm::ivec2& v) const {
        return 7919 * std::hash<int>()(v.x) + std::hash<int>()(v.y);
    }
};

std::vector<SearchNode> neighboors( 
        const SearchNode& node, 
        const std::vector<CityRect>& rects,
        const glm::vec3& final_pos
) {
    std::vector<SearchNode> neighs;

    static const std::array<std::pair<glm::ivec2, float>, 8> directions{
        std::pair<glm::ivec2, float>{{ 1,  0}, 1.0},
        std::pair<glm::ivec2, float>{{-1,  0}, 1.0},
        std::pair<glm::ivec2, float>{{ 0,  1}, 1.0},
        std::pair<glm::ivec2, float>{{ 0, -1}, 1.0},
        std::pair<glm::ivec2, float>{{ 1,  1}, SEARCH_SQRT2},
        std::pair<glm::ivec2, float>{{-1,  1}, SEARCH_SQRT2},
        std::pair<glm::ivec2, float>{{ 1, -1}, SEARCH_SQRT2},
        std::pair<glm::ivec2, float>{{-1, -1}, SEARCH_SQRT2}
    };

    glm::vec2 prev_dir{0.0};
    if( node.prev_state ) {
        prev_dir = glm::normalize(glm::vec2{ node.state.x-node.prev_state->x, node.state.y - node.prev_state->y });
    }

    for( const auto& direction : directions ) {
        auto candidate = node.state + direction.first;
        bool discard = false;
        for( const auto& rect : rects ) {
            if( rect.inside( candidate.x, candidate.y ) ) {
                discard = true;
                break;
            }
        }
        if( discard ) {
            continue;
        }


        auto new_dir = glm::normalize(glm::vec2{ candidate.x - node.state.x, candidate.y - node.state.y });
        float penalization = 1-( glm::dot(new_dir, prev_dir) );

        float h = glm::l2Norm( final_pos, glm::vec3{ (float)candidate.x, 0.0, (float)candidate.y } );
        neighs.emplace_back( candidate, node.state, node.g+direction.second+penalization, h );
    }

    return neighs;
} 

std::vector<Destination> search( 
        const Position& initial_pos,
        const Place& final_destination,
        const std::vector<Place>& places
) {
    std::vector<Destination> path;

    glm::vec3 final_pos{ final_destination.pos_x, 0.0, final_destination.pos_z };

    auto initial_state = State{ (int)initial_pos.value.x, (int)initial_pos.value.z };
    float initial_h = glm::l2Norm( final_pos, glm::vec3{ (float)initial_state.x, 0.0, (float)initial_state.y } );
    std::optional<CityRect> initial_rect;
    for( const auto& place : places ) {
        if( place.rect ) {
            if( place.rect->inside( initial_state.x, initial_state.y ) ) {
                initial_rect = place.rect;
                break;
            }
        }
    }
    std::vector<CityRect> rects;
    for( const auto& place : places  ) {
        if( place.rect && place.rect != final_destination.rect && (!initial_rect || place.rect != *initial_rect) ) {
            rects.emplace_back( *place.rect );
        }
    }

    std::priority_queue<SearchNode> open_list{};
    std::unordered_map<State, SearchNode, StateHash> closed_list;


    open_list.emplace( initial_state, std::optional<State>{}, 0, initial_h );
    closed_list.insert( {initial_state, open_list.top()} );

    //spdlog::debug("Searching...");

    //spdlog::debug("Initial node prev: {}", (bool)open_list.top().prev_state);
    //spdlog::debug("Initial state: {} {}", initial_state.first, initial_state.second);

    std::optional<State> last_state;
    while( !open_list.empty() ) {
        auto current_node = open_list.top();
        //spdlog::debug("Current node: {} {} {}", current_node.state.first, current_node.state.second, current_node.g);
        open_list.pop();

        // check goal
        if( final_destination.rect ) {
            if( final_destination.rect->inside( current_node.state.x, current_node.state.y ) ) {
                last_state = current_node.state;
                break;
            }
        } else {
            if( final_destination.pos_x >= (float)current_node.state.x && 
                final_destination.pos_x < (float)(current_node.state.x+1) &&
                final_destination.pos_z >= (float)current_node.state.y && 
                final_destination.pos_z < (float)(current_node.state.y+1) 
              ) {
                last_state = current_node.state;
                break;
            }
        }

        for( const auto& neighboor : neighboors( current_node, rects, final_pos ) ) {
            //spdlog::debug("\tNeighboor node: {} {} {}", neighboor.state.first, neighboor.state.second, neighboor.g);
            if( closed_list.find( neighboor.state ) == closed_list.end() ) { // || neighboor.f() < closed_list.at(neighboor.state).f() ) {
                open_list.emplace( neighboor );
                closed_list.insert( {neighboor.state, neighboor} );
            }
            //spdlog::debug("\t\t\tClosed list size: {}", closed_list.size());
        }
    }

    //spdlog::debug("Found solution! Nodes expanded: {}/{}", closed_list.size(), open_list.size());

    // recreate solution
    path.emplace_back( Destination{ glm::vec3{final_destination.pos_x, 0.0, final_destination.pos_z} } );
    while( last_state ) {
        //spdlog::debug("Current step: {} {}", last_state->x, last_state->y );
        auto prev_state = closed_list.at( *last_state ).prev_state;
        if( prev_state ) {
            glm::vec3 last_state_vec{ (float)last_state->x+0.5f, 0.0, (float)last_state->y+0.5f };
            glm::vec3 prev_state_vec{ (float)prev_state->x+0.5f, 0.0, (float)prev_state->y+0.5f };
            path.emplace_back( (last_state_vec+prev_state_vec)/2.f );
        }
        last_state = prev_state;
    }


    return path;
}
