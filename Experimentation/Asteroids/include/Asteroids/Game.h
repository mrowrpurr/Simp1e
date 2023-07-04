#pragma once

#include <Simp1e/CanvasComponent.h>
#include <Simp1e/LocalEngine.h>
#include <Simp1e/QtEngine.h>
#include <Simp1e/WindowComponent.h>
#include <_Log_.h>

using namespace Simp1e;

namespace Asteroids {

    class Game {
        LocalEngine _engine;
        QtEngine    _qtEngine{&_engine};

    public:
        Entity CreateWindowEntity(LocalEntityManager& entityManager) {
            auto window = entityManager.CreateEntity();
            entityManager.Add<WindowComponent>(window, "Asteroids");
            return window;
        }

        Entity CreateGameCanvas(Entity window, LocalEntityManager& entityManager) {
            auto canvas = entityManager.CreateEntity();
            entityManager.Add<CanvasComponent>(canvas, window);
            return canvas;
        }

        void LoadComponents(LocalEntityManager& entityManager) {
            // TODO: load from filesystem
            //
            auto window = CreateWindowEntity(entityManager);
            auto canvas = CreateGameCanvas(window, entityManager);
        }

        void Setup() {
            _engine.AddDefaultSystemGroups();
            LoadComponents(_engine.GetEntityManagerInstance());
        }

        void Run() {
            Setup();
            _qtEngine.Run();
        }
    };
}
