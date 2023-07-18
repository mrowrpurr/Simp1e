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
#include <cmath>

using namespace Simp1e;

namespace Asteroids {

    class ShipMovementSystem {
        Entity ship;    // TODO note could lookup from singleton tag component instead
        Entity camera;  // TODO Same deal...

    public:
        DEFINE_SYSTEM_TYPE("Asteroids:ShipMovementSystem");

        ShipMovementSystem(Entity ship, Entity camera) : ship(ship), camera(camera) {}

        // TODO: move angle and distance to Point struct

        double calculateAngle(Point p1, Point p2) {
            double dx           = p2.x() - p1.x();
            double dy           = p2.y() - p1.y();
            double angleRadians = std::atan2(dy, dx);
            double angleDegrees = angleRadians * 180.0 / M_PI;
            return angleDegrees;
        }

        double calculateDistance(Point p1, Point p2) {
            double dx = p2.x() - p1.x();
            double dy = p2.y() - p1.y();
            return std::sqrt(dx * dx + dy * dy);
        }

        PositionF heldInHandOffset = PositionF(-6, 0, -1);

        Point GetPositionDeltaFromAccelerometer(IEngine* engine) {
            auto accelerometerReading = engine->GetInput()->GetSensors()->ReadAccelerometer();
            if (accelerometerReading.IsNull()) {
                _Log_("No accelerometer reading");
                return {};
            }

            _Log_("ACCELEROMETER: {}", accelerometerReading.ToString());

            // Position laidFlatOffset(0, 0, 10);
            accelerometerReading = accelerometerReading + heldInHandOffset;

            auto movementTolerance = 1.0;
            // auto movementMax       = 100;
            // auto movementMax       = 100;
            auto movementMax = 5;

            // Left: -10.0
            // Right: 10.0
            auto xDelta = accelerometerReading.y();  // The Y in landscape would change the X on the screen
            if (xDelta < movementTolerance && xDelta > -movementTolerance) xDelta = 0;
            auto xMovement = movementMax * xDelta;  // Make this one more sensitive

            auto yDelta = accelerometerReading.x();  // The X in landscape would change the Y on the screen
            if (yDelta < movementTolerance && yDelta > -movementTolerance) yDelta = 0;
            auto yMovement = movementMax * yDelta;

            return Point(xMovement, yMovement);
        }

        PointF VelocityFromAccelerometer(IEngine* engine) {
            auto accelerometerReading = engine->GetInput()->GetSensors()->ReadAccelerometer();
            if (accelerometerReading.IsNull()) {
                _Log_("No accelerometer reading");
                return {};
            }
            // accelerometerReading   = accelerometerReading + heldInHandOffset;
            // auto movementTolerance = 1.0;

            // Calculate horizontal velocity (x)
            auto xSpeed = std::abs(accelerometerReading.y()) / 10.0;  // 0 to 10

            // Calculate vertical velocity (y)
            auto ySpeed = std::abs(accelerometerReading.x()) / 10.0;  // 0 to 10

            _Log_("Reading for SPEED: {}", accelerometerReading.ToString());
            _Log_("SPEED: {}", PointF(xSpeed, ySpeed).ToString());
            return PointF(xSpeed, ySpeed);

            // 0, 0, 10
            // 5, 0, 8

            // fwd = 5 to 0
            // back = 5 to 10
            // left = x, 0 to -10
            // right = x, 0 to 10
        }

        // FAKE ROTATION VERSION
        void Update(IEngine* engine, float) {
            //
            auto delta = GetPositionDeltaFromAccelerometer(engine);
            _Log_("MOVEMENT DELTA: {}", delta.ToString());

            if (delta.IsNull()) return;

            // Forget rotation for now, just move the ship and the camera...
            auto* positionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(ship);

            positionComponent->SetPosition(positionComponent->GetPosition() + delta);

            auto* cameraSize   = engine->GetEntities()->GetComponent<ISizeComponent>(camera);
            auto  shipPosition = positionComponent->GetPosition();

            // Center the ship in the camera view
            Point cameraPosition(
                shipPosition.x() - cameraSize->GetSize().width() / 2,
                shipPosition.y() - cameraSize->GetSize().height() / 2
            );
            auto* cameraPositionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(camera);
            cameraPositionComponent->SetPosition(cameraPosition);
            auto* cameraComponent = engine->GetEntities()->GetComponent<ICameraComponent>(camera);
            cameraComponent->SetDirty(true);  // Currently how it works, TODO fix and base on the position change ?
        }

        // Accelerometer version
        void Update_REAL_ROTATION(IEngine* engine, float) {
            auto positionDeltaPoint = GetPositionDeltaFromAccelerometer(engine);
            _Log_("MOVEMENT DELTA: {}", positionDeltaPoint.ToString());

            if (positionDeltaPoint.IsNull()) return;

            auto velocityFromAccelerometer = VelocityFromAccelerometer(engine);

            auto* rotationComponent                    = engine->GetEntities()->GetComponent<IRotationComponent>(ship);
            auto* positionComponent                    = engine->GetEntities()->GetComponent<IPositionComponent>(ship);
            auto  shipCurrentPoint                     = positionComponent->GetPosition().ToPoint();
            auto  calculatedNewPositionWithoutRotation = shipCurrentPoint + positionDeltaPoint;

            auto angle = calculateAngle(shipCurrentPoint, calculatedNewPositionWithoutRotation) + 180;
            _Log_(
                "Angle from {} to {} ---> {}", shipCurrentPoint.ToString(),
                calculatedNewPositionWithoutRotation.ToString(), angle
            );
            // if (angle < -3.0) angle = -3.0;
            // else if (angle > 3.0) angle = 3.0;
            auto currentRotation = rotationComponent->GetRotation();
            auto difference      = angle - currentRotation;
            if (difference > 3.0) angle = currentRotation + 3.0;
            else if (difference < -3.0) angle = currentRotation - 3.0;
            if (angle != 0) rotationComponent->RotateTo(angle);

            auto movementDistance = std::abs(calculateDistance(shipCurrentPoint, positionDeltaPoint));
            if (movementDistance > 10.0) movementDistance = 10.0;

            if (movementDistance != 0) {
                auto* positionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(ship);
                // auto  angleRadians      = rotationComponent->GetRotation();
                auto rotation     = rotationComponent->GetRotation();
                auto angleRadians = (90.0 - rotation) * M_PI / 180.0;

                //
                Point positionDelta(
                    cos(angleRadians) * (50 * velocityFromAccelerometer.x()),
                    sin(angleRadians) * (50 * -velocityFromAccelerometer.y())
                );
                // Point positionDelta(cos(angleRadians) * movementDistance, sin(angleRadians) * -movementDistance);

                positionComponent->SetPosition(positionComponent->GetPosition() + positionDelta);

                // Update the camera position...
                auto* cameraSize   = engine->GetEntities()->GetComponent<ISizeComponent>(camera);
                auto  shipPosition = positionComponent->GetPosition();

                // Center the ship in the camera view
                Point cameraPosition(
                    shipPosition.x() - cameraSize->GetSize().width() / 2,
                    shipPosition.y() - cameraSize->GetSize().height() / 2
                );
                auto* cameraPositionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(camera);
                cameraPositionComponent->SetPosition(cameraPosition);
                auto* cameraComponent = engine->GetEntities()->GetComponent<ICameraComponent>(camera);
                cameraComponent->SetDirty(true);  // Currently how it works, TODO fix and base on the position change ?
            }
        }

        // Accelerometer version
        void Update_NoRotate_Mobile(IEngine* engine, float) {
            auto delta = GetPositionDeltaFromAccelerometer(engine);
            _Log_("MOVEMENT DELTA: {}", delta.ToString());

            if (delta.IsNull()) return;

            // Forget rotation for now, just move the ship and the camera...
            auto* positionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(ship);

            positionComponent->SetPosition(positionComponent->GetPosition() + delta);

            auto* cameraSize   = engine->GetEntities()->GetComponent<ISizeComponent>(camera);
            auto  shipPosition = positionComponent->GetPosition();

            // Center the ship in the camera view
            Point cameraPosition(
                shipPosition.x() - cameraSize->GetSize().width() / 2,
                shipPosition.y() - cameraSize->GetSize().height() / 2
            );
            auto* cameraPositionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(camera);
            cameraPositionComponent->SetPosition(cameraPosition);
            auto* cameraComponent = engine->GetEntities()->GetComponent<ICameraComponent>(camera);
            cameraComponent->SetDirty(true);  // Currently how it works, TODO fix and base on the position change ?
        }

        void Update_Keyboard(IEngine* engine, float) {
            auto* keyboardInputManager = engine->GetInput()->GetKeyboard();
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

                //
                Point positionDelta(cos(angleRadians) * movementDistance, sin(angleRadians) * -movementDistance);

                positionComponent->SetPosition(positionComponent->GetPosition() + positionDelta);

                // Update the camera position...
                auto* cameraSize   = engine->GetEntities()->GetComponent<ISizeComponent>(camera);
                auto  shipPosition = positionComponent->GetPosition();

                // Center the ship in the camera view
                Point cameraPosition(
                    shipPosition.x() - cameraSize->GetSize().width() / 2,
                    shipPosition.y() - cameraSize->GetSize().height() / 2
                );
                auto* cameraPositionComponent = engine->GetEntities()->GetComponent<IPositionComponent>(camera);
                cameraPositionComponent->SetPosition(cameraPosition);
                auto* cameraComponent = engine->GetEntities()->GetComponent<ICameraComponent>(camera);
                cameraComponent->SetDirty(true);  // Currently how it works, TODO fix and base on the position change ?
            }
        }
    };

    class Game {
        Entity      _ship;
        Entity      _camera;
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
            // entityManager()->AddComponent<RectangleComponent>(camera);
            // entityManager()->AddComponent<LineColorComponent>(camera, Color::Magenta(10));
            _camera = camera;
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
            entityManager()->AddComponent<LineColorComponent>(ship, Color::Blue(40));
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

        std::unique_ptr<IFunctionPointer<PositionF()>> _readSensorsFunction;

        void SetupSensors() {
#ifdef SIMP1E_MOBILE
            _readSensorsFunction = unique_function_pointer(&_qtEngine, &QtEngine::GetAccelerometerReading);
            _engine.Input().Sensors().SetAccelerometerReadFunction(_readSensorsFunction.get());
            _qtEngine.UseAccelerometer();
#endif
        }

        void SetupSystems() {
            _engine.AddDefaultSystemGroups();
            _engine.GetSystemGroups()
                ->GetGroup(DefaultSystemGroupTypes::InitializationGroup)
                ->AddSystem<ShipMovementSystem>(_ship, _camera);
        }

        void Setup() {
            SetupSensors();
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
