#include "game_state.hpp"

#include "replicator/engine.hpp"

int main() {

    auto engine = Engine{};
    engine.set_window_size(800, 600);
    engine.set_window_title("Replicator");
    engine.set_aa(16);

    engine.bind_key( Key::Escape, "Close" );

    engine.bind_key( Key::Right, "CameraMoveRight" );
    engine.bind_key( Key::Left, "CameraMoveLeft" );
    engine.bind_key( Key::Up, "CameraMoveForward" );
    engine.bind_key( Key::Down, "CameraMoveBackward" );

    GameState state;
    engine.run(&state);
    
    return 0;
}
