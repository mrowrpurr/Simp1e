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

#include <QKeyEvent>

using namespace Simp1e;

Entity    _shipEntity;
Entity    _parallaxBackgroundEntity;
Entity    _viewPortPreviewRectangleEntity;
Direction _shipDirection = Direction::None;

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
        if (direction == Direction::North) currentShipPosition.SetY(currentShipPosition.y() - 50);
        else if (direction == Direction::South) currentShipPosition.SetY(currentShipPosition.y() + 50);
        else if (direction == Direction::East) currentShipPosition.SetX(currentShipPosition.x() + 50);
        else if (direction == Direction::West) currentShipPosition.SetX(currentShipPosition.x() - 50);

        shipPosition->SetPosition(currentShipPosition);
        parallaxBackground->SetTargetPerspectivePosition(currentShipPosition);
        viewportPreviewPosition->SetPosition(currentShipPosition);
    }

public:
    DEFINE_SYSTEM_TYPE("MoveStuff")

    void Update(IEngine* engine, float deltaTime) {
        if (_shipDirection == Direction::None) return;
        Move(engine, _shipDirection);
        _shipDirection = Direction::None;
    }
};

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
            parallax->AddLayer("Layer1", "C:/Users/mrowr/Downloads/galaxy-2357504_1920.jpg", 2.0, 1.0, 0.4);
            // parallax->AddLayer("Layer1", "C:/Code/mrowrpurr/StockImages/shutterstock_1921487843.svg", 2.0, 1.0, 0.4);
            // parallax->AddLayer(
            //     "Layer2", "C:/Code/mrowrpurr/StockImages/shutterstock_354953852 - without background.svg", 0.4, 1.0,
            //     0.7
            // );
            // parallax->AddLayer(
            //     "Layer3", "C:/Code/mrowrpurr/StockImages/shutterstock_2314360105 - without background.png", 1.5, 1.0,
            //     0.7
            // );
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
            _parallaxBackgroundEntity       = CreateParallaxEffect(entityManager);
            _viewPortPreviewRectangleEntity = CreateViewPortRectangle(entityManager);
            _shipEntity                     = CreateShip(entityManager);
        }

        void RegisterKeyboardHandlers() {
            _qtEngine.GetQtGuiSystem()->OnKeyPress(new_function_pointer([this](QKeyEvent* event) {
                _Log_("ON KEY EVENT");
                if (event->key() == Qt::Key_Left) _shipDirection = Direction::West;
                else if (event->key() == Qt::Key_Right) _shipDirection = Direction::East;
                else if (event->key() == Qt::Key_Up) _shipDirection = Direction::North;
                else if (event->key() == Qt::Key_Down) _shipDirection = Direction::South;
            }));
        }

        void Setup() {
            _engine.AddDefaultSystemGroups();
            _engine.SystemGroups().GetGroup(DefaultSystemGroupTypes::SimulationGroup)->AddSystem<MoveStuff>();
            LoadComponents(_engine.Entities());
            RegisterKeyboardHandlers();
        }

        void Run() {
            Setup();
            _qtEngine.Run();
        }
    };
}
