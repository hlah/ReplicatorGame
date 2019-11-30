#ifndef _REPLICATOR_GAME_BEZIER_H_
#define _REPLICATOR_GAME_BEZIER_H_

#include "glm/vec3.hpp"
#include <vector>
#include <cmath>

class Curve {
    public:
        inline void add_segment( glm::vec3 point_start, glm::vec3 tangent_start, glm::vec3 tangent_end, glm::vec3 point_end ) {
            _segments.push_back( {point_start, tangent_start, tangent_end, point_end} );
        }
        inline glm::vec3 get_point( float t ) const {
            auto segment_index = (size_t)floor(t) % _segments.size();
            t = t-floor(t);
            auto segment = _segments[segment_index];
            return (1-t)*(1-t)*(1-t)*segment.point_start
                 + 3*t*(1-t)*(1-t)*segment.tangent_start
                 + 3*t*t*(1-t)*segment.tangent_end
                 + t*t*t*segment.point_end;
        }
        inline float tmax() const { return (float)_segments.size(); }
    private:
        struct Segment {
            glm::vec3 point_start;
            glm::vec3 tangent_start;
            glm::vec3 tangent_end;
            glm::vec3 point_end;
        };
        std::vector<Segment> _segments;
};


#endif // _REPLICATOR_GAME_BEZIER_H_
