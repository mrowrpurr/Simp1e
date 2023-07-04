#include <Simp1e/LocalEngine.h>

#include "Asteroids/Game.h"

Simp1e::LocalEngine engine;
Asteroids::Game     game{&engine};

int main(int argc, char* argv[]) {
    game.Run();
    return 0;
}
