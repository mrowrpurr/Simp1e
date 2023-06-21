#include <Simp1e/QT/_Log_.h>
//

#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/KeyboardInputSystem.h>
#include <Simp1e/ECS/MouseClickInputSystem.h>
#include <Simp1e/ECS/OnKeyboardComponent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTImageComponent.h>
#include <Simp1e/ECS/QTImageComponentRenderer.h>
#include <Simp1e/ECS/QtPositionComponentUpdateHandler.h>
#include <Simp1e/ECS/QtRectangleComponentRenderer.h>
#include <Simp1e/ECS/QtRenderSystem.h>
#include <Simp1e/ECS/QtSizeComponentUpdateHandler.h>
#include <Simp1e/ECS/QtTextComponentRenderer.h>
#include <Simp1e/ECS/QtTextComponentUpdateHandler.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/ResizeNotificationSystem.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/Size.h>
#include <string_format.h>

#include <QApplication>
#include <QTimer>

#include "Simp1e/SideScroller/QtViewCenteredSystem.h"
#include "Simp1e/SideScroller/SideScrollerGraphicsScene.h"
#include "Simp1e/SideScroller/SideScrollerGraphicsView.h"
#include "Simp1e/SideScroller/ViewCenteredComponent.h"

using namespace Simp1e;
using namespace Simp1e::ECS;

Game   game;
Size   levelSize{8000, 600};
QTimer mainLoopTimer;

int  loopIteration = 0;
void GameLoop() {
    loopIteration++;
    game.Update();
}

void SetupQtRenderSystem(Game& game, QGraphicsScene& scene) {
    auto* qtRenderSystem = new QtRenderSystem(game, scene);
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, QtPositionComponentUpdateHandler>();
    qtRenderSystem->AddComponentUpdateHandler<TextComponent, QtTextComponentUpdateHandler>();
    qtRenderSystem->AddComponentUpdateHandler<SizeComponent, QtSizeComponentUpdateHandler>();
    qtRenderSystem->AddComponentRenderer<RectangleComponent, QtRectangleComponentRenderer>();
    qtRenderSystem->AddComponentRenderer<QTImageComponent, QTImageComponentRenderer>();
    qtRenderSystem->AddComponentRenderer<TextComponent, QtTextComponentRenderer>();
    game.Systems().AddSystem(qtRenderSystem);
}

constexpr auto RIGHT_IMAGE   = ":/player/images/look/right.png";
constexpr auto LEFT_IMAGE    = ":/player/images/look/left.png";
bool           isMoving      = false;
bool           isMovingRight = false;
ManagedEntity  AddPlayer(Game& game, const QRectF& sceneRect) {
    Size size{100, 100};
    auto player = game.Entities().CreateEntity();
    player.AddComponent<ViewCenteredComponent>();
    auto* position = player.AddComponent<PositionComponent>(
        {static_cast<sreal>(sceneRect.width() / 2 - 50), static_cast<sreal>(sceneRect.height() / 2 - 50)}
    );
    auto* sizeComponent = player.AddComponent<SizeComponent>(size);
    auto* image         = player.AddComponent<QTImageComponent>({RIGHT_IMAGE});
    player.AddComponent<OnKeyboardComponent>({[position, sizeComponent, image](KeyboardEvent* e) {
        if (!e->pressed()) return;
        // if (e->repeated()) return;
        if (e->key() == KeyboardEvent::Key::Left) {
            position->SetX(position->x() - 10);
            image->SetPixmap(LEFT_IMAGE);
        } else if (e->key() == KeyboardEvent::Key::Right) {
            position->SetX(position->x() + 10);
            image->SetPixmap(RIGHT_IMAGE);
        } else if (e->key() == KeyboardEvent::Key::Up) position->SetY(position->y() - 10);
        else if (e->key() == KeyboardEvent::Key::Down) position->SetY(position->y() + 10);
    }});
    // player.AddComponent<RectangleComponent>({Color::Magenta(20)});
    player.AddComponent<OnMouseClickComponent>({[position, image](MouseClickEvent* e) {
        if (!e->pressed()) {
            isMoving = false;
            return;
        }
        if (!isMoving) {
            isMoving      = true;
            isMovingRight = e->x() > position->x();
        }
        if (isMovingRight) {
            position->SetX(position->x() + 10);
            image->SetPixmap(RIGHT_IMAGE);
        } else {
            position->SetX(position->x() - 10);
            image->SetPixmap(LEFT_IMAGE);
        }
    }});

    return player;
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("fusion");

    SideScrollerGraphicsScene scene(game.Events());
    scene.setSceneRect(0, 0, levelSize.width(), levelSize.height());

    SideScrollerGraphicsView view(game.Events());
    view.setWindowTitle("Side Scroller");
    view.setScene(&scene);
    view.setGeometry(0, 0, scene.sceneRect().width(), scene.sceneRect().height());
    view.resize(1600, 900);

    SetupQtRenderSystem(game, scene);

    auto* commandSystem = game.Systems().AddSystem<CommandSystem>();

    KeyboardInputSystem keyboardInputSystem(game.Entities().GetEntityManager());
    keyboardInputSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&keyboardInputSystem);

    MouseClickInputSystem mouseClickInputSystem(game.Entities().GetEntityManager());
    mouseClickInputSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&mouseClickInputSystem);

    ResizeNotificationSystem resizeNotificationSystem(game.Entities().GetEntityManager());
    resizeNotificationSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&resizeNotificationSystem);

    QtViewCenteredSystem viewCenteredSystem(view, game.Entities().GetEntityManager());
    game.Systems().AddSystem(&viewCenteredSystem);

    auto backgroundRectangle = game.Entities().CreateEntity();
    backgroundRectangle.AddComponent<PositionComponent>({0, 0});
    backgroundRectangle.AddComponent<SizeComponent>({levelSize.width(), levelSize.height()});
    backgroundRectangle.AddComponent<RectangleComponent>({Color::Magenta(20)});

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

    AddPlayer(game, scene.sceneRect());

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);

    view.show();
    return app.exec();
}
