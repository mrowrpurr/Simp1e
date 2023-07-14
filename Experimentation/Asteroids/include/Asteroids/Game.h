#pragma once

#include <Simp1e/CameraComponent.h>
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

#include <QAccelerometerReading>
#include <QEvent>
#include <QKeyEvent>
#include <QTabletEvent>
#include <QTouchEvent>

using namespace Simp1e;

namespace Asteroids {

    class Game {
        LocalEngine _engine;
        QtEngine    _qtEngine{&_engine};

        LocalEntityManager& entityManager() { return _engine.Entities(); }

        Entity CreateWindowEntity() {
            auto window = entityManager().CreateEntity();
            entityManager().AddComponent<WindowComponent>(window, "Asteroids (Simp1e prototype)");
            return window;
        }

        Entity CreateCanvas() {
            auto canvas = entityManager().CreateEntity();
            entityManager().AddComponent<CanvasComponent>(canvas);
            return canvas;
        }

        Entity CreateCamera() {
            auto camera = entityManager().CreateEntity();
            entityManager().AddComponent<CameraComponent>(camera);
            // TODO: position and size
            return camera;
        }

        Entity CreateParallaxEffect() {
            auto parallaxEffect = entityManager().CreateEntity();
            entityManager().AddComponent<SizeComponent>(parallaxEffect);
            auto parallax = entityManager().AddComponent<ParallaxEffectComponent>(parallaxEffect);
            parallax->AddLayer("Layer1", ":/background-layer-1.jpg", 2.0f, 1.0f, 0.4f);
            parallax->AddLayer("Layer2", ":/background-layer-2.png", 0.4f, 3.0f, 0.7f);
            parallax->AddLayer("Layer3", ":/background-layer-3.png", 1.5f, 1.0f, 0.4f);
            return parallaxEffect;
        }

        Entity CreateShip() {
            auto ship = entityManager().CreateEntity();
            entityManager().AddComponent<SizeComponent>(ship, Size(50, 75));
            entityManager().AddComponent<PositionComponent>(ship);
            entityManager().AddComponent<RotationComponent>(ship);
            entityManager().AddComponent<ImageComponent>(ship, ":/ship.png");
            return ship;
        }

        // TODO: load components from filesystem
        void LoadComponents(LocalEntityManager& entityManager) {
            CreateWindowEntity();
            CreateCanvas();
            CreateShip();
        }

        void Setup() { LoadComponents(_engine.Entities()); }

    public:
        void Run() {
            Setup();
            _qtEngine.Run();
        }
    };
}
