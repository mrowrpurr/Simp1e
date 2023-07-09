#pragma once

#include <Simp1e/CanvasComponent.h>
#include <Simp1e/FillColorComponent.h>
#include <Simp1e/ImageComponent.h>
#include <Simp1e/LineColorComponent.h>
#include <Simp1e/LocalEngine.h>
#include <Simp1e/PositionComponent.h>
#include <Simp1e/QtEngine.h>
#include <Simp1e/RectangleComponent.h>
#include <Simp1e/RotationComponent.h>
#include <Simp1e/Size.h>
#include <Simp1e/SizeComponent.h>
#include <Simp1e/WindowComponent.h>
#include <_Log_.h>

#include <QLabel>

using namespace Simp1e;

#include <Simp1e/QtEngine.h>

#include <QLabel>

namespace Asteroids {

    class Game {
        LocalEngine _engine;
        QtEngine    _qtEngine{&_engine};
        Size        _size{20000, 20000};

    public:
        Entity CreateWindowEntity(LocalEntityManager& entityManager) {
            auto window = entityManager.CreateEntity();
            entityManager.AddComponent<WindowComponent>(window, "Asteroids");
            return window;
        }

        Entity CreateGameCanvas(Entity window, LocalEntityManager& entityManager) {
            auto canvas = entityManager.CreateEntity();
            entityManager.AddComponent<CanvasComponent>(canvas, window, _size.width(), _size.height());
            entityManager.AddComponent<SizeComponent>(canvas, _size);
            return canvas;
        }

        Entity CreateBackground(LocalEntityManager& entityManager) {
            auto background = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(background);
            entityManager.AddComponent<SizeComponent>(background, _size);
            entityManager.AddComponent<RectangleComponent>(background);
            entityManager.AddComponent<FillColorComponent>(background, Color::Pink());
            return background;
        }

        Entity CreateTopLeftRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(rectangle);
            entityManager.AddComponent<SizeComponent>(rectangle, Size(100, 100));
            entityManager.AddComponent<RectangleComponent>(rectangle);
            entityManager.AddComponent<FillColorComponent>(rectangle, Color::Red());
            return rectangle;
        }

        Entity CreateTopRightRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(rectangle, _size.width() - 100, 0);
            entityManager.AddComponent<SizeComponent>(rectangle, Size(100, 100));
            entityManager.AddComponent<RectangleComponent>(rectangle);
            entityManager.AddComponent<FillColorComponent>(rectangle, Color::Green());
            return rectangle;
        }

        Entity CreateBottomLeftRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(rectangle, 0, _size.height() - 100);
            entityManager.AddComponent<SizeComponent>(rectangle, Size(100, 100));
            entityManager.AddComponent<RectangleComponent>(rectangle);
            entityManager.AddComponent<FillColorComponent>(rectangle, Color::Blue());
            return rectangle;
        }

        Entity CreateBottomRightRectangle(LocalEntityManager& entityManager) {
            auto rectangle = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(rectangle, _size.width() - 100, _size.height() - 100);
            entityManager.AddComponent<SizeComponent>(rectangle, Size(100, 100));
            entityManager.AddComponent<RectangleComponent>(rectangle);
            entityManager.AddComponent<FillColorComponent>(rectangle, Color::Yellow());
            return rectangle;
        }

        Entity CreateShip(LocalEntityManager& entityManager) {
            auto ship = entityManager.CreateEntity();
            entityManager.AddComponent<SizeComponent>(ship, Size(100, 125));
            entityManager.AddComponent<PositionComponent>(ship, _size.width() / 2, _size.height() / 2);
            entityManager.AddComponent<RotationComponent>(ship);
            entityManager.AddComponent<ImageComponent>(ship, ":/ship.png");
            return ship;
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
            CreateShip(entityManager);
        }

        void Setup() {
            _engine.AddDefaultSystemGroups();
            LoadComponents(_engine.Entities());
        }

        void Run() {
            Setup();
            _qtEngine.Run();
        }
    };
}
