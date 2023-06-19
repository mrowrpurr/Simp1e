#include <Simp1e/QT/_Log_.h>
//

// TODO rename all of my Qt* files and classes to QT* so it doesn't collide with Qt's classes

#include <Simp1e/Direction.h>
#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/CommandTypeMacro.h>
#include <Simp1e/ECS/Conversions/ToKeyboardEvent.h>
#include <Simp1e/ECS/Conversions/ToMouseClickEvent.h>
#include <Simp1e/ECS/EventTypeMacro.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/KeyboardEvent.h>
#include <Simp1e/ECS/KeyboardInputSystem.h>
#include <Simp1e/ECS/MouseClickEvent.h>
#include <Simp1e/ECS/MouseClickInputSystem.h>
#include <Simp1e/ECS/OnKeyboardComponent.h>
#include <Simp1e/ECS/OnMouseClickComponent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTImageComponent.h>
#include <Simp1e/ECS/QTImageComponentRenderer.h>
#include <Simp1e/ECS/QtPositionComponentUpdateHandler.h>
#include <Simp1e/ECS/QtRectangleComponentRenderer.h>
#include <Simp1e/ECS/QtRenderSystem.h>
#include <Simp1e/ECS/QtTextComponentRenderer.h>
#include <Simp1e/ECS/QtTextComponentUpdateHandler.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/ResizeEvent.h>
#include <Simp1e/ECS/ResizeNotificationSystem.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/VisibleComponent.h>
#include <Simp1e/QT/Conversions/ToQString.h>

#include <QApplication>
#include <QElapsedTimer>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QLabel>
#include <QMouseEvent>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>
#include <functional>

using namespace Simp1e;
using namespace Simp1e::ECS;

class SideScrollerGraphicsScene : public QGraphicsScene {
    EventManager& eventManager;

public:
    SideScrollerGraphicsScene(EventManager& eventManager, QObject* parent = nullptr)
        : QGraphicsScene(parent), eventManager(eventManager) {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        eventManager.Emit<MouseClickEvent>(ToMouseClickEvent(event, true));
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        eventManager.Emit<MouseClickEvent>(ToMouseClickEvent(event, false));
    }
};

class SideScrollerGraphicsView : public QGraphicsView {
    EventManager& _eventManager;
    QLabel        fpsLabel;
    QElapsedTimer fpsTimer;
    int           frames = 0;

public:
    SideScrollerGraphicsView(EventManager& eventManager) : _eventManager(eventManager) {
        setRenderHint(QPainter::Antialiasing);  // Enable smooth rendering.
        fpsTimer.start();
    }

    QLabel& FPSLabel() { return fpsLabel; }

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override {
        QGraphicsView::drawBackground(painter, rect);
        ++frames;
        if (fpsTimer.elapsed() > 1000) {
            fpsTimer.restart();
            fpsLabel.setText(QString("FPS: %1").arg(frames));
            frames = 0;
        }
    }

    void keyPressEvent(QKeyEvent* event) override {
        _eventManager.Emit<KeyboardEvent>(ToKeyboardEvent(event, true));
        QGraphicsView::keyPressEvent(event);
    }
    void keyReleaseEvent(QKeyEvent* event) override {
        _eventManager.Emit<KeyboardEvent>(ToKeyboardEvent(event, false));
        QGraphicsView::keyReleaseEvent(event);
    }
    void resizeEvent(QResizeEvent* event) override {
        fitInView(sceneRect(), Qt::KeepAspectRatio);
        auto topLeft = mapToScene(0, 0);
        _eventManager.Emit<ResizeEvent>({
            {static_cast<sreal>(topLeft.x()),           static_cast<sreal>(topLeft.y())           },
            {static_cast<sreal>(event->size().width()), static_cast<sreal>(event->size().height())}
        }
        );
        QGraphicsView::resizeEvent(event);
    }
};

void SetupQtRenderSystem(Game& game, QGraphicsScene& scene) {
    auto* qtRenderSystem = new QtRenderSystem(game, scene);

    qtRenderSystem->AddVisualComponentType<VisibleComponent>();

    // Component update handlers
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, QtPositionComponentUpdateHandler>(
    );
    qtRenderSystem->AddComponentUpdateHandler<TextComponent, QtTextComponentUpdateHandler>();

    // Renderers (order matters)
    qtRenderSystem->AddComponentRenderer<RectangleComponent, QtRectangleComponentRenderer>();
    qtRenderSystem->AddComponentRenderer<QTImageComponent, QTImageComponentRenderer>();
    qtRenderSystem->AddComponentRenderer<TextComponent, QtTextComponentRenderer>();

    game.Systems().AddSystem(qtRenderSystem);
}

class SetImageCommand {
    QTImageComponent* _imageComponent;
    std::string       _imageFilePath;

public:
    SIMP1E_ECS_COMMAND("SetImage")

    SetImageCommand(QTImageComponent* imageComponent, const std::string& imageFilePath)
        : _imageComponent(imageComponent), _imageFilePath(imageFilePath) {}

    void Execute() { _imageComponent->SetPixmap(ToQString(_imageFilePath)); }
};

class MovePositionCommand {
    PositionComponent* position;
    Direction          direction;
    sreal              distance;

public:
    SIMP1E_ECS_COMMAND("MovePosition")

    MovePositionCommand(PositionComponent* position, Direction direction, sreal distance)
        : position(position), direction(direction), distance(distance) {}

    void Execute() {
        if (direction == Direction::North) position->SetY(position->y() - distance);
        else if (direction == Direction::South) position->SetY(position->y() + distance);
        else if (direction == Direction::West) position->SetX(position->x() - distance);
        else if (direction == Direction::East) position->SetX(position->x() + distance);
    }
};

ManagedEntity AddPlayer(Game& game, const QRectF& sceneRect) {
    auto player = game.Entities().CreateEntity();
    Size size{100, 100};
    // auto* position = player.AddComponent<PositionComponent>({static_cast<sreal>(left), 0});
    auto* position = player.AddComponent<PositionComponent>(
        {static_cast<sreal>(sceneRect.width() / 2 - size.width() / 2),
         static_cast<sreal>(sceneRect.height() - size.height())}
    );
    player.AddComponent<SizeComponent>(size);
    player.AddComponent<QTImageComponent>({":/player/images/look/right.png"});
    // TODO update to send a MovePositionCommand
    player.AddComponent<OnKeyboardComponent>({[position](KeyboardEvent* e) {
        if (e->key() == KeyboardEvent::Key::Left) position->SetX(position->x() - 10);
        else if (e->key() == KeyboardEvent::Key::Right) position->SetX(position->x() + 10);
        else if (e->key() == KeyboardEvent::Key::Up) position->SetY(position->y() - 10);
        else if (e->key() == KeyboardEvent::Key::Down) position->SetY(position->y() + 10);
    }});
    // player.AddComponent<RectangleComponent>({Color::Magenta()});
    return player;
}

bool IsMouseOver(MouseClickEvent* event, PositionComponent* position, SizeComponent* size) {
    return event->x() >= position->x() && event->x() <= position->x() + size->width() &&
           event->y() >= position->y() && event->y() <= position->y() + size->height();
}

void AddMoveLeftButton(
    Game& game, const Size& sceneSize, CommandSystem* commandSystem,
    PositionComponent* playerPosition, QTImageComponent* playerImage
) {
    auto  button   = game.Entities().CreateEntity();
    auto* position = button.AddComponent<PositionComponent>({0, 0});
    auto* size = button.AddComponent<SizeComponent>({sceneSize.width() / 2, sceneSize.height()});

    button.AddComponent<OnResizeComponent>({[size, position](ResizeEvent* e) {
        size->SetWidth(e->width() / 2);
        size->SetHeight(e->height());
        position->SetPosition({e->x(), e->y()});
    }});

    button.AddComponent<OnMouseClickComponent>({[commandSystem, playerPosition, playerImage,
                                                 position, size](MouseClickEvent* e) {
        if (IsMouseOver(e, position, size)) {
            commandSystem->AddCommand<MovePositionCommand>({playerPosition, Direction::West, 5});
            commandSystem->AddCommand<SetImageCommand>(
                {playerImage, ":/player/images/look/left.png"}
            );
        }
    }});
}

void AddMoveRightButton(
    Game& game, const Size& sceneSize, CommandSystem* commandSystem,
    PositionComponent* playerPosition, QTImageComponent* playerImage
) {
    auto  button   = game.Entities().CreateEntity();
    auto* position = button.AddComponent<PositionComponent>({sceneSize.width() / 2, 0});
    auto* size = button.AddComponent<SizeComponent>({sceneSize.width() / 2, sceneSize.height()});

    button.AddComponent<OnResizeComponent>({[size, position](ResizeEvent* e) {
        qDebug() << "Resize event. Width: " << e->width() << " Height: " << e->height()
                 << " x: " << e->x() << " y: " << e->y();
        size->SetWidth(e->width() / 2);
        size->SetHeight(e->height());
        position->SetPosition({e->x() + e->width() / 2, e->y()});
    }});

    button.AddComponent<OnMouseClickComponent>({[commandSystem, playerPosition, playerImage,
                                                 position, size](MouseClickEvent* e) {
        if (IsMouseOver(e, position, size)) {
            commandSystem->AddCommand<MovePositionCommand>({playerPosition, Direction::East, 5});
            commandSystem->AddCommand<SetImageCommand>(
                {playerImage, ":/player/images/look/right.png"}
            );
        }
    }});
}

Game    game;
QTimer  mainLoopTimer;
QLabel* loopIterationLabel;

int  loopIteration = 0;
void GameLoop() {
    loopIteration++;
    loopIterationLabel->setText(QString("Loop Iteration: %1").arg(loopIteration));
    game.Update();
}

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("fusion");
    QWidget window;
    // make the background color of the QWidget red
    window.setWindowTitle("Side Scroller");
    loopIterationLabel = new QLabel();
    auto* layout       = new QVBoxLayout();
    window.setLayout(layout);
    SideScrollerGraphicsView view(game.Events());
    // set the background color of the view to blue
    SideScrollerGraphicsScene scene(game.Events());
    Size                      gameSize{1600, 800};
    QRectF                    sceneRect(0, 0, gameSize.width(), gameSize.height());
    view.setFixedSize(gameSize.width(), gameSize.height());
    view.setScene(&scene);
    view.setGeometry(sceneRect.toRect());
    scene.setSceneRect(sceneRect);
    view.fitInView(sceneRect, Qt::KeepAspectRatio);
    layout->addWidget(&view.FPSLabel());
    layout->addWidget(loopIterationLabel);
    layout->addWidget(&view);
    window.show();
    ///////////////////////

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

    auto  player         = AddPlayer(game, sceneRect);
    auto* playerPosition = player.GetComponent<PositionComponent>();
    auto* playerImage    = player.GetComponent<QTImageComponent>();
    AddMoveLeftButton(game, gameSize, commandSystem, playerPosition, playerImage);
    AddMoveRightButton(game, gameSize, commandSystem, playerPosition, playerImage);

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);

    ///////////////////////
    return app.exec();
}

// bool viewportEvent(QEvent* event) override {
//     switch (event->type()) {
//         case QEvent::TouchBegin: {
//             QTouchEvent*                   touchEvent  = static_cast<QTouchEvent*>(event);
//             QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->points();
//             _touchTimer.restart();
//             _recentTouchPos = _currentTouchPos = touchPoints.first().pressPosition();
//         }