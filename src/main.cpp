#include "game_state.hpp"

#include "replicator/engine.hpp"

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");
    engine.set_aa(16);

    engine.bind_button( Key::Escape, "Close" );

    engine.bind_button( Key::D, "CameraMoveRight" );
    engine.bind_button( Key::A, "CameraMoveLeft" );
    engine.bind_button( Key::W, "CameraMoveForward" );
    engine.bind_button( Key::S, "CameraMoveBackward" );
    engine.bind_button( Key::F, "CameraUp" );
    engine.bind_button( Key::V, "CameraDown" );
    engine.bind_button( MouseButton::Right, "MoveCamera" );
    engine.bind_button( MouseButton::Left, "Select" );

    GameState state;
    engine.run(&state);
    
    return 0;
}
