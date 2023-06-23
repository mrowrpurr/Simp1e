#include <Simp1e/QT/_Log_.h>
//

#include <Simp1e/Direction.h>
#include <Simp1e/ECS/CollisionComponent.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/GravityComponent.h>
#include <Simp1e/ECS/GravitySystem.h>
#include <Simp1e/ECS/JumpCommand.h>
#include <Simp1e/ECS/JumpSystem.h>
#include <Simp1e/ECS/JumpingComponent.h>
#include <Simp1e/ECS/KeyboardInputSystem.h>
#include <Simp1e/ECS/MouseClickInputSystem.h>
#include <Simp1e/ECS/OnKeyboardComponent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QtCollisionSystem.h>
#include <Simp1e/ECS/QtImageComponentRenderer.h>
#include <Simp1e/ECS/QtPositionComponentUpdateHandler.h>
#include <Simp1e/ECS/QtRectangleComponentRenderer.h>
#include <Simp1e/ECS/QtRenderSystem.h>
#include <Simp1e/ECS/QtSizeComponentUpdateHandler.h>
#include <Simp1e/ECS/QtTextComponentRenderer.h>
#include <Simp1e/ECS/QtTextComponentUpdateHandler.h>
#include <Simp1e/ECS/QtViewCenteredSystem.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/ResizeNotificationSystem.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/ECS/StoreLastPositionSystem.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/ViewCenteredComponent.h>
#include <Simp1e/Size.h>
#include <string_format.h>

#include <QApplication>
#include <QTimer>

#include "Simp1e/SideScroller/Images.h"
#include "Simp1e/SideScroller/MovePlayerCommand.h"
#include "Simp1e/SideScroller/SideScrollerGraphicsScene.h"
#include "Simp1e/SideScroller/SideScrollerGraphicsView.h"

using namespace Simp1e;
using namespace Simp1e::ECS;

Game   game;
Size   levelSize{10000, 1000};
QTimer mainLoopTimer;

void GameLoop() {
    game.Update();

    // Run any remaining commands before the next game loop
    game.Systems().GetSystem<CommandSystem>()->Update();
}

void SetupQtRenderSystem(Game& game, QGraphicsScene& scene) {
    auto* qtRenderSystem = new QtRenderSystem(game, scene);
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, QtPositionComponentUpdateHandler>();
    qtRenderSystem->AddComponentUpdateHandler<TextComponent, QtTextComponentUpdateHandler>();
    qtRenderSystem->AddComponentUpdateHandler<SizeComponent, QtSizeComponentUpdateHandler>();
    qtRenderSystem->AddComponentRenderer<RectangleComponent, QtRectangleComponentRenderer>();
    qtRenderSystem->AddComponentRenderer<ImageComponent, QtImageComponentRenderer>();
    qtRenderSystem->AddComponentRenderer<TextComponent, QtTextComponentRenderer>();
    game.Systems().AddSystem(qtRenderSystem);
}

bool          isMoving      = false;
bool          isMovingRight = false;
ManagedEntity AddPlayer(Game& game, CommandSystem& commandSystem, const QRectF& sceneRect) {
    Size size{250, 250};
    auto player = game.Entities().CreateEntity();
    player.AddComponent<ViewCenteredComponent>();
    player.AddComponent<CollisionComponent>({true});
    player.AddComponent<GravityComponent>({{.gravityFactor = 10.0}});
    auto* position = player.AddComponent<PositionComponent>(
        {static_cast<sreal>(sceneRect.width() / 2 - 50), static_cast<sreal>(sceneRect.height() / 2 - 50)}
    );
    auto* sizeComponent = player.AddComponent<SizeComponent>(size);
    auto* image         = player.AddComponent<ImageComponent>({Images::Player::Look::Right});
    player.AddComponent<OnKeyboardComponent>({[player, position, sizeComponent, image,
                                               &commandSystem](KeyboardEvent* e) {
        if (!e->pressed()) return;
        if (e->key() == KeyboardEvent::Key::Left || e->key() == KeyboardEvent::Key::KeyA)
            commandSystem.AddCommand<MovePlayerCommand>({player, Direction::West, 10});
        else if (e->key() == KeyboardEvent::Key::Right || e->key() == KeyboardEvent::Key::KeyD)
            commandSystem.AddCommand<MovePlayerCommand>({player, Direction::East, 10});
        else if (e->key() == KeyboardEvent::Key::Space) {
            qDebug() << "Jump";
            commandSystem.RunCommand<JumpCommand>(player, 300, 20);
        }
    }});
    player.AddComponent<OnMouseClickComponent>({[player, position, image, sizeComponent,
                                                 &commandSystem](MouseClickEvent* e) {
        if (!e->pressed()) {
            isMoving = false;
            return;
        }
        bool isJumping = e->y() < position->y() - sizeComponent->size().height() / 2;
        if (isJumping) {
            commandSystem.RunCommand<JumpCommand>(player, 300, 20);
            return;
        }
        if (!isMoving) {
            isMoving      = true;
            isMovingRight = e->x() > position->x();
        }
        if (isMovingRight) commandSystem.AddCommand<MovePlayerCommand>({player, Direction::East, 10});
        else commandSystem.AddCommand<MovePlayerCommand>({player, Direction::West, 10});
    }});

    return player;
}

int main(int argc, char* argv[]) {
    /*
        Qt UI
    */

    QApplication app(argc, argv);
    app.setStyle("fusion");

    SideScrollerGraphicsScene scene(game.Events());
    scene.setSceneRect(0, 0, levelSize.width(), levelSize.height());

    SideScrollerGraphicsView view(game.Events());
    view.setWindowTitle("Side Scroller");
    view.setScene(&scene);

    /*
        Systems
    */

    StoreLastPositionSystem storeLastPositionSystem(game.Entities().GetEntityManager());
    game.Systems().AddSystem(&storeLastPositionSystem);

    CommandSystem commandSystem(game);
    game.Systems().AddSystem(&commandSystem);

    JumpSystem jumpSystem(game.Entities().GetEntityManager(), commandSystem);
    game.Systems().AddSystem(&jumpSystem);

    GravitySystem gravitySystem(game.Entities().GetEntityManager(), commandSystem);
    game.Systems().AddSystem(&gravitySystem);

    KeyboardInputSystem keyboardInputSystem(game.Entities().GetEntityManager());
    keyboardInputSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&keyboardInputSystem);

    MouseClickInputSystem mouseClickInputSystem(game.Entities().GetEntityManager());
    mouseClickInputSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&mouseClickInputSystem);

    ResizeNotificationSystem resizeNotificationSystem(game.Entities().GetEntityManager());
    resizeNotificationSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&resizeNotificationSystem);

    QtCollisionSystem qtCollisionSystem(game.Entities().GetEntityManager(), game.Events(), commandSystem, scene);
    game.Systems().AddSystem(&qtCollisionSystem);

    SetupQtRenderSystem(game, scene);

    QtViewCenteredSystem viewCenteredSystem(view, game.Entities().GetEntityManager());
    game.Systems().AddSystem(&viewCenteredSystem);

    /*
        Components
    */

    auto backgroundRectangle = game.Entities().CreateEntity();
    backgroundRectangle.AddComponent<PositionComponent>({0, 0});
    backgroundRectangle.AddComponent<SizeComponent>({levelSize.width(), levelSize.height()});
    backgroundRectangle.AddComponent<RectangleComponent>({Color::Magenta(20)});

    auto topBar = game.Entities().CreateEntity();
    topBar.AddComponent<PositionComponent>({0, 0});
    topBar.AddComponent<SizeComponent>({levelSize.width(), 50});
    topBar.AddComponent<RectangleComponent>({Color::Blue(50)});

    auto bottomBar = game.Entities().CreateEntity();
    bottomBar.AddComponent<PositionComponent>({0, levelSize.height() - 50});
    bottomBar.AddComponent<SizeComponent>({levelSize.width(), 50});
    bottomBar.AddComponent<RectangleComponent>({Color::Blue(100)});
    bottomBar.AddComponent<CollisionComponent>();

    auto topLeftRectangle = game.Entities().CreateEntity();
    topLeftRectangle.AddComponent<PositionComponent>({0, 0});
    topLeftRectangle.AddComponent<SizeComponent>({100, 100});
    topLeftRectangle.AddComponent<RectangleComponent>({Color::Red(20)});
    topLeftRectangle.AddComponent<TextComponent>({"0x0"});

    auto topRightRectangle = game.Entities().CreateEntity();
    topRightRectangle.AddComponent<PositionComponent>({levelSize.width() - 100, 0});
    topRightRectangle.AddComponent<SizeComponent>({100, 100});
    topRightRectangle.AddComponent<RectangleComponent>({Color::Green(20)});
    topRightRectangle.AddComponent<TextComponent>({string_format("{}x{}", levelSize.width(), 0)});

    auto bottomLeftRectangle = game.Entities().CreateEntity();
    bottomLeftRectangle.AddComponent<PositionComponent>({0, levelSize.height() - 100});
    bottomLeftRectangle.AddComponent<SizeComponent>({100, 100});
    bottomLeftRectangle.AddComponent<RectangleComponent>({Color::Blue(20)});
    bottomLeftRectangle.AddComponent<TextComponent>({string_format("{}x{}", 0, levelSize.height())});

    auto bottomRightRectangle = game.Entities().CreateEntity();
    bottomRightRectangle.AddComponent<PositionComponent>({levelSize.width() - 100, levelSize.height() - 100});
    bottomRightRectangle.AddComponent<SizeComponent>({100, 100});
    bottomRightRectangle.AddComponent<RectangleComponent>({Color::Yellow(20)});
    bottomRightRectangle.AddComponent<TextComponent>({string_format("{}x{}", levelSize.width(), levelSize.height())});

    auto middleRectangle = game.Entities().CreateEntity();
    middleRectangle.AddComponent<PositionComponent>({levelSize.width() / 2 - 50, levelSize.height() / 2 - 50});
    middleRectangle.AddComponent<SizeComponent>({100, 100});
    middleRectangle.AddComponent<RectangleComponent>({Color::Cyan(20)});
    middleRectangle.AddComponent<TextComponent>({string_format("{}x{}", levelSize.width() / 2, levelSize.height() / 2)}
    );

    AddPlayer(game, commandSystem, scene.sceneRect());

    /*
        Start game main() loop and run the application
    */

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);
    view.show();
    return app.exec();
}
