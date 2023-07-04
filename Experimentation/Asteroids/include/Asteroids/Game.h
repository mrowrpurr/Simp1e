#pragma once

#include <Simp1e/CanvasComponent.h>
#include <Simp1e/FillColorComponent.h>
#include <Simp1e/LocalEngine.h>
#include <Simp1e/PositionComponent.h>
#include <Simp1e/QtEngine.h>
#include <Simp1e/RectangleComponent.h>
#include <Simp1e/Size.h>
#include <Simp1e/SizeComponent.h>
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
            entityManager.Add<SizeComponent>(canvas, _size);
            return canvas;
        }

        Entity CreateBackground(LocalEntityManager& entityManager) {
            auto background = entityManager.CreateEntity();
            entityManager.Add<PositionComponent>(background);
            entityManager.Add<SizeComponent>(background, _size);
            entityManager.Add<RectangleComponent>(background);
            entityManager.Add<FillColorComponent>(background, Color::Pink());
            return background;
        }

        Entity CreateTopLeftRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.Add<PositionComponent>(rectangle);
            entityManager.Add<SizeComponent>(rectangle, Size(100, 100));
            entityManager.Add<RectangleComponent>(rectangle);
            entityManager.Add<FillColorComponent>(rectangle, Color::Red());
            return rectangle;
        }

        Entity CreateTopRightRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.Add<PositionComponent>(rectangle, _size.width() - 100, 0);
            entityManager.Add<SizeComponent>(rectangle, Size(100, 100));
            entityManager.Add<RectangleComponent>(rectangle);
            entityManager.Add<FillColorComponent>(rectangle, Color::Green());
            return rectangle;
        }

        Entity CreateBottomLeftRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.Add<PositionComponent>(rectangle, 0, _size.height() - 100);
            entityManager.Add<SizeComponent>(rectangle, Size(100, 100));
            entityManager.Add<RectangleComponent>(rectangle);
            entityManager.Add<FillColorComponent>(rectangle, Color::Blue());
            return rectangle;
        }

        Entity CreateBottomRightRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.Add<PositionComponent>(rectangle, _size.width() - 100, _size.height() - 100);
            entityManager.Add<SizeComponent>(rectangle, Size(100, 100));
            entityManager.Add<RectangleComponent>(rectangle);
            entityManager.Add<FillColorComponent>(rectangle, Color::Yellow());
            return rectangle;
        }

        // TODO: load components from filesystem
        void LoadComponents(LocalEntityManager& entityManager) {
            auto window = CreateWindowEntity(entityManager);
            CreateGameCanvas(window, entityManager);
            CreateBackground(entityManager);
            CreateTopLeftRectangle(entityManager);
            CreateTopRightRectangle(entityManager);
            CreateBottomLeftRectangle(entityManager);
            CreateBottomRightRectangle(entityManager);
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
