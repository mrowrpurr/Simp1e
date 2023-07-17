#pragma once

#include <Simp1e/CameraComponent.h>
#include <Simp1e/CanvasComponent.h>
#include <Simp1e/EventCast.h>
#include <Simp1e/FillColorComponent.h>
#include <Simp1e/FromKeyboardKey.h>
#include <Simp1e/ImageComponent.h>
#include <Simp1e/KeyboardEvent.h>
#include <Simp1e/KeyboardKey.h>
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

#include <QEvent>
#include <QKeyEvent>
#include <QTabletEvent>
#include <QTouchEvent>

using namespace Simp1e;

namespace Asteroids {

    class ShipMovementSystem {
        Entity ship;  // TODO note could lookup from singleton tag component instead

    public:
        DEFINE_SYSTEM_TYPE("Asteroids:ShipMovementSystem");

        ShipMovementSystem(Entity ship) : ship(ship) {}

        void Update(IEngine* engine, float) {
            auto* keyboardInputManager = engine->GetInput()->GetKeyboardInputManager();
            auto* rotationComponent    = engine->GetEntities()->GetComponent<IRotationComponent>(ship);

            int rotationDelta = 0;
            if (keyboardInputManager->IsKeyPressed(FromKeyboardKey(KeyboardKey::Left))) rotationDelta = -1;
            if (keyboardInputManager->IsKeyPressed(FromKeyboardKey(KeyboardKey::Right))) rotationDelta = 1;
            if (rotationDelta != 0) rotationComponent->Rotate(rotationDelta);

            auto movementDistance = 0.0;
            if (keyboardInputManager->IsKeyPressed(FromKeyboardKey(KeyboardKey::Up))) movementDistance = 5.0;

            if (movementDistance != 0) {
                auto* positionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(ship);
                auto  rotation          = rotationComponent->GetRotation();
                auto  angleRadians      = (90.0 - rotation) * M_PI / 180.0;
                Point positionDelta(cos(angleRadians) * movementDistance, sin(angleRadians) * -movementDistance);
                positionComponent->SetPosition(positionComponent->GetPosition() + positionDelta);
            }
        }
    };

    class Game {
        Entity      _ship;
        LocalEngine _engine;
        QtEngine    _qtEngine{&_engine};

        IEntityManager* entityManager() { return _engine.GetEntities(); }
        IEventManager*  eventManager() { return _engine.GetEvents(); }

        void CreateWindowEntity() {
            auto window = entityManager()->CreateEntity();
            entityManager()->AddComponent<SizeComponent>(window, Size(1600, 1000));
            entityManager()->AddComponent<WindowComponent>(window, "Asteroids (Simp1e prototype)");
        }

        void CreateCanvas() {
            auto canvas = entityManager()->CreateEntity();
            entityManager()->AddComponent<CanvasComponent>(canvas);
            entityManager()->AddComponent<SizeComponent>(canvas);
        }

        void CreateCamera() {
            auto camera = entityManager()->CreateEntity();
            entityManager()->AddComponent<CameraComponent>(camera);
            entityManager()->AddComponent<PositionComponent>(camera, Position(0, 0));
            entityManager()->AddComponent<SizeComponent>(camera, Size(1000, 1000));
            entityManager()->AddComponent<RectangleComponent>(camera);
            entityManager()->AddComponent<LineColorComponent>(camera, Color::Magenta());
        }

        void CreateParallaxEffect() {
            auto parallaxEffect = entityManager()->CreateEntity();
            entityManager()->AddComponent<SizeComponent>(parallaxEffect);
            auto parallax = entityManager()->AddComponent<ParallaxEffectComponent>(parallaxEffect);
            parallax->AddLayer("Layer1", ":/background-layer-1.jpg", 2.0f, 1.0f, 0.4f);
            parallax->AddLayer("Layer2", ":/background-layer-2.png", 0.4f, 3.0f, 0.7f);
            parallax->AddLayer("Layer3", ":/background-layer-3.png", 1.5f, 1.0f, 0.4f);
        }

        void CreateStars() {
            auto starsBackground = entityManager()->CreateEntity();
            entityManager()->AddComponent<SizeComponent>(starsBackground, 8000, 3330);
            entityManager()->AddComponent<PositionComponent>(starsBackground);
            entityManager()->AddComponent<ImageComponent>(starsBackground, ":/background-stars.png");
            entityManager()->AddComponent<RectangleComponent>(starsBackground);
            entityManager()->AddComponent<LineColorComponent>(starsBackground, Color::White());
        }

        void CreateShip() {
            auto ship = entityManager()->CreateEntity();
            entityManager()->AddComponent<SizeComponent>(ship, Size(0, 150));
            entityManager()->AddComponent<PositionComponent>(ship, Position(100, 100));
            entityManager()->AddComponent<RotationComponent>(ship);
            entityManager()->AddComponent<ImageComponent>(ship, ":/ship.png");
            entityManager()->AddComponent<LineColorComponent>(ship, Color::Blue());
            _ship = ship;
        }

        // TODO: load components from filesystem
        void LoadComponents() {
            CreateWindowEntity();
            CreateCanvas();
            CreateCamera();
            CreateStars();
            CreateShip();
        }

        void SetupSystems() {
            _engine.AddDefaultSystemGroups();
            _engine.GetSystemGroups()
                ->GetGroup(DefaultSystemGroupTypes::InitializationGroup)
                ->AddSystem<ShipMovementSystem>(_ship);
        }

        void Setup() {
            LoadComponents();
            SetupSystems();
        }

    public:
        void Run() {
            Setup();
            _qtEngine.Run();
        }
    };
}
