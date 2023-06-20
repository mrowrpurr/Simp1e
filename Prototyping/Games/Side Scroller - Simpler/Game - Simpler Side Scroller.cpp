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
#include <Simp1e/ECS/QtSizeComponentUpdateHandler.h>
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

Size GetGameSize(int labelHeight = 0) {
    qDebug() << "GetGameSize() label height: " << labelHeight;
    // #ifdef Q_OS_ANDROID
    auto screenGeometry = QGuiApplication::primaryScreen()->geometry();
    return {
        static_cast<sreal>(screenGeometry.width()),
        static_cast<sreal>(screenGeometry.height() - labelHeight)};
    // #else
    //     return {1600, 800};
    // #endif
}

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

        // show scroll bars
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);

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
        qDebug() << "RESIZE";

        int newWidth  = event->size().width();
        int newHeight = event->size().height();

        // setFixedSize(newHeight - 50, newWidth - 50);

        // Calculate the scaling factor based on the height.
        // qreal scaleFactor = static_cast<qreal>(newHeight) / scene()->sceneRect().height();
        qreal scaleFactor = static_cast<qreal>(newHeight) / scene()->sceneRect().height();

        // Apply the transformation.
        QTransform transform;
        transform.scale(scaleFactor, scaleFactor);
        setTransform(transform);

        qDebug() << "Transform has been set.";

        auto topLeft = mapToScene(0, 0);
        _eventManager.Emit<ResizeEvent>({
            {static_cast<sreal>(topLeft.x()),           static_cast<sreal>(topLeft.y())           },
            {static_cast<sreal>(event->size().width()), static_cast<sreal>(event->size().height())}
        }
        );
        for (auto item : scene()->items()) item->update();
        qDebug() << "Scene items have been updated";
    }
};

void SetupQtRenderSystem(Game& game, QGraphicsScene& scene) {
    auto* qtRenderSystem = new QtRenderSystem(game, scene);

    qtRenderSystem->AddVisualComponentType<VisibleComponent>();

    // Component update handlers
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, QtPositionComponentUpdateHandler>(
    );
    qtRenderSystem->AddComponentUpdateHandler<TextComponent, QtTextComponentUpdateHandler>();
    qtRenderSystem->AddComponentUpdateHandler<SizeComponent, QtSizeComponentUpdateHandler>();

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
        qDebug() << "MOVE PLAYER";
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
    // auto* position = player.AddComponent<PositionComponent>(
    //     {static_cast<sreal>(sceneRect.width() / 2 - size.width() / 2),
    //      static_cast<sreal>(sceneRect.height() - size.height())}
    // );
    // auto* position = player.AddComponent<PositionComponent>({500, 600});
    auto* position = player.AddComponent<PositionComponent>(
        {static_cast<sreal>(sceneRect.width() / 2 - 50),
         static_cast<sreal>(sceneRect.height() / 2 - 50)}
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

// TODO XXX TODO - update the movement click/tap handlers to move the player based on if the
// position is to the right or left of the PLAYER.
int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("fusion");
    QWidget window;
    window.setWindowTitle("Side Scroller");
    loopIterationLabel = new QLabel();
    loopIterationLabel->setMaximumHeight(20);
    auto* layout = new QVBoxLayout();
    window.setLayout(layout);
    SideScrollerGraphicsView  view(game.Events());
    SideScrollerGraphicsScene scene(game.Events());
    view.FPSLabel().setMaximumHeight(20);
    auto gameSize = GetGameSize(view.FPSLabel().height() + loopIterationLabel->height());
    qDebug() << "Game size width " << gameSize.width() << " height " << gameSize.height();
    QRectF sceneRect(0, 0, gameSize.width(), gameSize.height());
    scene.setSceneRect(sceneRect);
    view.setFixedSize(gameSize.width() - 100, gameSize.height() - 100);
    view.setScene(&scene);
    view.setGeometry(sceneRect.toRect());
    view.fitInView(sceneRect, Qt::KeepAspectRatio);
    layout->addWidget(&view.FPSLabel());
    layout->addWidget(loopIterationLabel);
    layout->addWidget(&view);
    window.show();

    auto* screen = QGuiApplication::primaryScreen();
    QObject::connect(screen, &QScreen::geometryChanged, &window, [&]() {

    });

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

    auto background = game.Entities().CreateEntity();
    background.AddComponent<PositionComponent>({0, 0});
    background.AddComponent<SizeComponent>({gameSize.width(), gameSize.height()});
    background.AddComponent<RectangleComponent>({Color::Green(10)});

    // auto topLeftRect = game.Entities().CreateEntity();
    // topLeftRect.AddComponent<PositionComponent>({0, 0});
    // topLeftRect.AddComponent<SizeComponent>({100, 100});
    // topLeftRect.AddComponent<RectangleComponent>({Color::Red()});

    auto middleRect = game.Entities().CreateEntity();
    middleRect.AddComponent<PositionComponent>(
        {gameSize.width() / 2 - 50, gameSize.height() / 2 - 50}
    );
    middleRect.AddComponent<SizeComponent>({100, 100});
    middleRect.AddComponent<RectangleComponent>({Color::Blue(69)});

    auto anotherRect = game.Entities().CreateEntity();
    anotherRect.AddComponent<PositionComponent>({600, 500});
    anotherRect.AddComponent<SizeComponent>({100, 100});
    anotherRect.AddComponent<RectangleComponent>({Color::Purple(69)});

    // auto bottomRightRect = game.Entities().CreateEntity();
    // bottomRightRect.AddComponent<PositionComponent>(
    //     {gameSize.width() - 100, gameSize.height() - 100}
    // );
    // bottomRightRect.AddComponent<SizeComponent>({100, 100});
    // bottomRightRect.AddComponent<RectangleComponent>({Color::Yellow()});

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