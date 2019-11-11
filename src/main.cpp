#include "game_state.hpp"

#include "replicator/engine.hpp"

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");
    engine.set_aa(16);

    engine.bind_key( Key::Escape, "Close" );

    engine.bind_key( Key::D, "CameraMoveRight" );
    engine.bind_key( Key::A, "CameraMoveLeft" );
    engine.bind_key( Key::W, "CameraMoveForward" );
    engine.bind_key( Key::S, "CameraMoveBackward" );
    engine.bind_key( Key::F, "CameraUp" );
    engine.bind_key( Key::V, "CameraDown" );

    GameState state;
    engine.run(&state);
    
    return 0;
}
