#ifndef _REPLICATOR_GAME_GAME_STATE_H_
#define _REPLICATOR_GAME_GAME_STATE_H_

#include "replicator/state.hpp"

class GameState : public State {
    public:
        virtual State::Transition on_start( entt::registry& registry ) override; 
        virtual State::Transition on_action( entt::registry& registry, const ActionEvent& action ) override; 
        virtual State::Transition on_mouse_move( entt::registry& registry, double mx, double my ) override; 
        virtual State::Transition update( entt::registry& registry ) override; 

    private:
        // Helper function for action that edit float values
        void handle_action( ActionEvent action, const std::string& action_name, float& value_to_edit, float value ); 

        entt::entity _player, _head;

        const float _player_rotation_speed = 0.100;
        float _player_y_rotation = 0.0;
        float _head_x_rotation = 0.0;

        const float _player_speed = 20.0;
        float _player_horizontal_v = 0.0;
        float _player_transversal_v = 0.0;
        float _player_vertical_v = 0.0;

        double _prev_mouse_x, _prev_mouse_y;
        double _new_mouse_x, _new_mouse_y;
};

#endif // _REPLICATOR_GAME_GAME_STATE_H_
