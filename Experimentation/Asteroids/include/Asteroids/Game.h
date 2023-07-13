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

#include <QAccelerometerReading>
#include <QEvent>
#include <QKeyEvent>
#include <QTabletEvent>
#include <QTouchEvent>

using namespace Simp1e;

QtEngine* _qtEnginePtr = nullptr;

qreal     _yReading    = 0;
bool      _sizeChanged = false;
bool      _isMoving    = false;
Entity    _shipEntity;
Entity    _parallaxBackgroundEntity;
Entity    _viewPortPreviewRectangleEntity;
Direction _shipDirection = Direction::None;
Size      _viewportSize{3840, 2160};

void UpdateViewportSize() {
    //
    auto screenGeometry = QGuiApplication::primaryScreen()->geometry();
    _viewportSize       = Size(screenGeometry.width(), screenGeometry.height());
}

// MAKE A SYSTEM FOR MOVING THE SHIP AND THE PARALLAX!
class MoveStuff {
    void Move(IEngine* engine, Direction direction) {
        // Ship position component
        auto* shipPosition = engine->GetEntities()->GetComponent<PositionComponent>(_shipEntity);

        // Parallax Background component
        auto* parallaxBackground =
            engine->GetEntities()->GetComponent<ParallaxEffectComponent>(_parallaxBackgroundEntity);

        // Rectangle viewport preview component
        auto* viewportPreviewPosition =
            engine->GetEntities()->GetComponent<PositionComponent>(_viewPortPreviewRectangleEntity);

        auto currentShipPosition = shipPosition->GetPosition();
        if (direction == Direction::North) currentShipPosition.SetY(currentShipPosition.y() - 2);
        else if (direction == Direction::South) currentShipPosition.SetY(currentShipPosition.y() + 2);
        else if (direction == Direction::East) currentShipPosition.SetX(currentShipPosition.x() + 2);
        else if (direction == Direction::West) currentShipPosition.SetX(currentShipPosition.x() - 2);

        shipPosition->SetPosition(currentShipPosition);
        parallaxBackground->SetTargetPerspectivePosition(Position(
            (currentShipPosition.x() - _viewportSize.width() / 2),
            (currentShipPosition.y() - _viewportSize.height() / 2)
        ));
        viewportPreviewPosition->SetPosition(currentShipPosition);
        if (_qtEnginePtr) _qtEnginePtr->GetQtGuiSystem()->CenterOnTheShip();
    }

public:
    DEFINE_SYSTEM_TYPE("MoveStuff")

    void Update(IEngine* engine, float deltaTime) {
        if (_yReading != 0) {
            if (_yReading <= -4.0 || _yReading >= 4.0) _qtEnginePtr->GetQtGuiSystem()->Rotate(_yReading / 5);
            _yReading = 0;
        }
        if (_sizeChanged) {
            _sizeChanged = false;
            // Parallax Background component
            auto* parallaxBackground =
                engine->GetEntities()->GetComponent<ParallaxEffectComponent>(_parallaxBackgroundEntity);
            auto* parallaxSize = engine->GetEntities()->GetComponent<SizeComponent>(_parallaxBackgroundEntity);
            parallaxSize->SetSize(_viewportSize);
            parallaxBackground->SetDirty(true);  // update!

            // Rectangle viewport preview component
            auto* viewportPreviewSize =
                engine->GetEntities()->GetComponent<SizeComponent>(_viewPortPreviewRectangleEntity);
            viewportPreviewSize->SetSize(_viewportSize);
        }
        //
        if (_isMoving) Move(engine, Direction::North);
        // if (_shipDirection == Direction::None) return;
        // Move(engine, _shipDirection);
        // _shipDirection = Direction::None;
    }
};

namespace Asteroids {

    class Game {
        LocalEngine _engine;
        QtEngine    _qtEngine{&_engine};
        // Size        _viewportSize{3840, 2160};

    public:
        Game() {
            _qtEnginePtr = &_qtEngine;
            // auto screenGeometry = QGuiApplication::primaryScreen()->geometry();
            // _viewportSize       = Size(screenGeometry.width(), screenGeometry.height());
            UpdateViewportSize();
        }

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
            // parallax->AddLayer(
            //     "Layer1", "C:/Code/mrowrpurr/StockImages/shutterstock_1921487843 - no borders.jpg", 2.0, 1.0, 0.4
            // );
            // parallax->AddLayer(
            //     "Layer2", "C:/Code/mrowrpurr/StockImages/shutterstock_354953852 - without background.png", 0.4, 3.0,
            //     0.7
            // );
            // parallax->AddLayer(
            //     "Layer3", "C:/Code/mrowrpurr/StockImages/shutterstock_2314360105 - without background-fs8.png", 1.5,
            //     1.0, 0.4
            // );
            parallax->AddLayer("Layer1", ":/background-layer-1.jpg", 2.0f, 1.0f, 0.4f);
            parallax->AddLayer("Layer2", ":/background-layer-2.png", 0.4f, 3.0f, 0.7f);
            parallax->AddLayer("Layer3", ":/background-layer-3.png", 1.5f, 1.0f, 0.4f);
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
            entityManager.AddComponent<LineColorComponent>(background, Color::Blue(10));
            return background;
        }

        Entity CreateShip(LocalEntityManager& entityManager) {
            auto ship = entityManager.CreateEntity();
            entityManager.AddComponent<SizeComponent>(ship, Size(100, 125));
            entityManager.AddComponent<PositionComponent>(
                ship, Point(_viewportSize.width() / 2, _viewportSize.height() / 2)
            );
            entityManager.AddComponent<RotationComponent>(ship);
            entityManager.AddComponent<ImageComponent>(ship, ":/ship.png");
            return ship;
        }

        Entity CreateShipPropulsion(LocalEntityManager& entityManager) {
            // ...
            return 0;  // TODO
        }

        // TODO: load components from filesystem
        void LoadComponents(LocalEntityManager& entityManager) {
            auto window = CreateWindowEntity(entityManager);
            CreateGameCanvas(window, entityManager);
            // CreateLargerThanViewPortBackground(entityManager);
            _parallaxBackgroundEntity       = CreateParallaxEffect(entityManager);
            _viewPortPreviewRectangleEntity = CreateViewPortRectangle(entityManager);
            _shipEntity                     = CreateShip(entityManager);
        }

        void RegisterKeyboardHandlers() {
            _qtEngine.GetQtGuiSystem()->OnKeyPress(new_function_pointer([](QKeyEvent* event) {
                _Log_("ON KEY EVENT");
                if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A) _shipDirection = Direction::West;
                else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D) _shipDirection = Direction::East;
                else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) _shipDirection = Direction::North;
                else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S) _shipDirection = Direction::South;
            }));
            _qtEngine.GetQtGuiSystem()->OnViewportEvent(new_function_pointer([this](QEvent* event) {
                _Log_("Viewport Event");
                _qtEngine.GetQtGuiSystem()->CenterOnTheShip();
                switch (event->type()) {
                    case QEvent::TouchBegin:
                        _Log_("TOUCH BEGIN");
                        break;
                    case QEvent::TouchUpdate:
                        _Log_("TOUCH UPDATE");
                        break;
                    case QEvent::TouchEnd:
                        _Log_("TOUCH END");
                        break;
                    case QEvent::TouchCancel:
                        _Log_("TOUCH CANCEL");
                        break;
                    case QEvent::MouseButtonPress:
                        _Log_("MOUSE BUTTON PRESS");
                        _isMoving = true;
                        break;
                    case QEvent::MouseButtonRelease:
                        _Log_("MOUSE BUTTON RELEASE");
                        _isMoving = false;
                        break;
                    default:
                        _Log_("Different viewport event");
                        break;
                }
            }));
            _qtEngine.GetQtGuiSystem()->OnAccelerometerReadingChanged(
                new_function_pointer([this](QAccelerometerReading* reading) {
                    // _Log_("Accelerometer reading changed");
                    // auto reading = _qtEngine.GetQtGuiSystem()->GetAccelerometerReading();
                    // _Log_(reading.x());
                    // _Log_(reading.y());
                    // _Log_(reading.z());
                    _yReading = reading->y();
                })
            );
        }

        void Setup() {
            _engine.AddDefaultSystemGroups();
            _engine.SystemGroups().GetGroup(DefaultSystemGroupTypes::SimulationGroup)->AddSystem<MoveStuff>();
            LoadComponents(_engine.Entities());
            RegisterKeyboardHandlers();
            _qtEngine.GetQtGuiSystem()->OnResize(new_function_pointer([this]() {
                //
                UpdateViewportSize();
                _sizeChanged = true;
                _qtEngine.GetQtGuiSystem()->CenterOnTheShip();
            }));
        }

        void Run() {
            Setup();
            _qtEngine.Run();
        }
    };
}
