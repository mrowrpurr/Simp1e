#pragma once

#include <Simp1e/CanvasComponent.h>
#include <Simp1e/LocalEngine.h>
#include <Simp1e/PositionComponent.h>
#include <Simp1e/QtEngine.h>
#include <Simp1e/Size.h>
#include <Simp1e/WindowComponent.h>
#include <_Log_.h>


using namespace Simp1e;

namespace Asteroids {

    class Game {
        LocalEngine _engine;
        QtEngine    _qtEngine{&_engine};
        Size        _size{2000, 1000};

    public:
        Entity CreateWindowEntity(LocalEntityManager& entityManager) {
            auto window = entityManager.CreateEntity();
            entityManager.Add<WindowComponent>(window, "Asteroids");
            return window;
        }

        Entity CreateGameCanvas(Entity window, LocalEntityManager& entityManager) {
            auto canvas = entityManager.CreateEntity();
            entityManager.Add<CanvasComponent>(canvas, window);
            // entityManager.Add<SizeComponent>(canvas, _size);
            return canvas;
        }

        Entity CreateBackground(LocalEntityManager& entityManager) {
            auto background = entityManager.CreateEntity();
            entityManager.Add<PositionComponent>(background);
            // entityManager.Add<SizeComponent>(background, _size);
            // entityManager.Add<RectangleComponent>(background);
            return background;
        }

        // TODO: load components from filesystem
        void LoadComponents(LocalEntityManager& entityManager) {
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
