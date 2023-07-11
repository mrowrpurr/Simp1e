#pragma once

#include <Simp1e/CanvasComponent.h>
#include <Simp1e/FillColorComponent.h>
#include <Simp1e/ImageComponent.h>
#include <Simp1e/LineColorComponent.h>
#include <Simp1e/LocalEngine.h>
#include <Simp1e/ParallaxEffectComponent.h>
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
        Size        _viewportSize{3840, 2160};

    public:
        Entity CreateWindowEntity(LocalEntityManager& entityManager) {
            auto window = entityManager.CreateEntity();
            entityManager.AddComponent<WindowComponent>(window, "Asteroids (Simp1e prototype)");
            return window;
        }

        Entity CreateGameCanvas(Entity window, LocalEntityManager& entityManager) {
            auto canvas = entityManager.CreateEntity();
            entityManager.AddComponent<CanvasComponent>(canvas, window);
            return canvas;
        }

        Entity CreateParallaxEffect(LocalEntityManager& entityManager) {
            auto parallaxEffect = entityManager.CreateEntity();
            entityManager.AddComponent<SizeComponent>(parallaxEffect, _viewportSize);
            auto parallax = entityManager.AddComponent<ParallaxEffectComponent>(parallaxEffect);
            parallax->AddLayer("Layer1", "C:/Code/mrowrpurr/StockImages/shutterstock_1921487843.svg", 0.5, 1.0, 0.3);
            parallax->AddLayer(
                "Layer2", "C:/Code/mrowrpurr/StockImages/shutterstock_579041497 - without background.svg", 1.0, 4.0, 0.5
            );
            parallax->AddLayer(
                "Layer3", "C:/Code/mrowrpurr/StockImages/shutterstock_354953852 - without background.svg", 2.0, 3.0, 0.7
            );
            return parallaxEffect;
        }

        Entity CreateLargerThanViewPortBackground(LocalEntityManager& entityManager) {
            auto background = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(background, Point(-10000, -10000));
            entityManager.AddComponent<SizeComponent>(background, Size(20000, 20000));
            entityManager.AddComponent<RectangleComponent>(background);
            entityManager.AddComponent<LineColorComponent>(background, Color::Magenta());
            return background;
        }

        Entity CreateViewPortRectangle(LocalEntityManager& entityManager) {
            auto background = entityManager.CreateEntity();
            entityManager.AddComponent<PositionComponent>(background);
            entityManager.AddComponent<SizeComponent>(background, _viewportSize);
            entityManager.AddComponent<RectangleComponent>(background);
            entityManager.AddComponent<LineColorComponent>(background, Color::Blue());
            return background;
        }

        Entity CreateShip(LocalEntityManager& entityManager) {
            auto ship = entityManager.CreateEntity();
            entityManager.AddComponent<SizeComponent>(ship, Size(100, 125));
            entityManager.AddComponent<PositionComponent>(ship, Point(0, 0));
            entityManager.AddComponent<RotationComponent>(ship);
            entityManager.AddComponent<ImageComponent>(ship, ":/ship.png");
            return ship;
        }

        Entity CreateShipPropulsion(LocalEntityManager& entityManager) {
            // ...
        }

        // TODO: load components from filesystem
        void LoadComponents(LocalEntityManager& entityManager) {
            auto window = CreateWindowEntity(entityManager);
            CreateGameCanvas(window, entityManager);
            CreateLargerThanViewPortBackground(entityManager);
            CreateParallaxEffect(entityManager);
            CreateViewPortRectangle(entityManager);
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
