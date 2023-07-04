#pragma once

#include <Simp1e/LocalEngine.h>
#include <Simp1e/QtGameLoop.h>
#include <Simp1e/QtGuiSystem.h>

using namespace Simp1e;

namespace Asteroids {

    class TestSystem {
    public:
        void Update(IEngine*, double) {}
    };

    class Game {
        bool         _setup = false;
        LocalEngine* _engine;

    public:
        Game(LocalEngine* engine) : _engine(engine) {}

        void Setup() { _engine->GetSystemManagerInstance().Add<TestSystem>("something"); }

        void Run() {
            if (!_setup) {
                Setup();
                _setup = true;
            }

            QtGameLoop gameLoop(_engine);
            gameLoop.Run();
        }
    };
}
