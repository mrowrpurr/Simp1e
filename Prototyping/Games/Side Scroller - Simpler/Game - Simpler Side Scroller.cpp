#include <Simp1e/QT/_Log_.h>
//

// TODO rename all of my Qt* files and classes to QT* so it doesn't collide with Qt's classes

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/Conversions/ToKeyboardEvent.h>
#include <Simp1e/ECS/Conversions/ToMouseClickEvent.h>
#include <Simp1e/ECS/EventTypeMacro.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/KeyboardEvent.h>
#include <Simp1e/ECS/MouseClickEvent.h>
#include <Simp1e/ECS/OnKeyboardEventComponent.h>
#include <Simp1e/ECS/OnMouseClickEventComponent.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTImageComponent.h>
#include <Simp1e/ECS/QTImageComponentRenderer.h>
#include <Simp1e/ECS/QtPositionComponentUpdateHandler.h>
#include <Simp1e/ECS/QtRectangleComponentRenderer.h>
#include <Simp1e/ECS/QtRenderSystem.h>
#include <Simp1e/ECS/QtTextComponentRenderer.h>
#include <Simp1e/ECS/QtTextComponentUpdateHandler.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/SizeComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/VisibleComponent.h>

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

class EventEmittingGraphicsScene : public QGraphicsScene {
    EventManager& eventManager;

public:
    EventEmittingGraphicsScene(EventManager& eventManager, QObject* parent = nullptr)
        : QGraphicsScene(parent), eventManager(eventManager) {}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        eventManager.Emit<MouseClickEvent>(ToMouseClickEvent(event, true));
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event) override {
        eventManager.Emit<MouseClickEvent>(ToMouseClickEvent(event, false));
    }
};

class GraphicsViewWithFPS : public QGraphicsView {
    EventManager& _eventManager;
    QLabel        fpsLabel;
    QElapsedTimer fpsTimer;
    int           frames = 0;

public:
    GraphicsViewWithFPS(EventManager& eventManager) : _eventManager(eventManager) {
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
    }
    void keyReleaseEvent(QKeyEvent* event) override {
        _eventManager.Emit<KeyboardEvent>(ToKeyboardEvent(event, false));
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

class KeyboardInputSystem {
    EntityManager&                 _entityManager;
    std::unique_ptr<KeyboardEvent> _lastKeyboardEvent;

public:
    KeyboardInputSystem(EntityManager& entityManager) : _entityManager(entityManager) {}

    SIMP1E_ECS_SYSTEM("KeyboardInputSystem")

    void RegisterListener(EventManager& eventManager) {
        eventManager.AddListener<KeyboardEvent>([this](KeyboardEvent* event) {
            OnKeyboardEvent(event);
        });
    }

    void Update() {
        if (!_lastKeyboardEvent) return;
        auto& onKeyboardInputComponents = _entityManager.GetComponents<OnKeyboardEventComponent>();
        for (auto& [entity, component] : onKeyboardInputComponents) {
            auto onKeyboardInputComponent = component_cast<OnKeyboardEventComponent>(component);
            onKeyboardInputComponent->TriggerEvent(_lastKeyboardEvent.get());
        }
        _lastKeyboardEvent.reset();
    }

protected:
    virtual void OnKeyboardEvent(KeyboardEvent* e) {
        _lastKeyboardEvent = std::make_unique<KeyboardEvent>(*e);
    }
};

void AddPlayer(Game& game) {
    auto  player   = game.Entities().CreateEntity();
    auto* position = player.AddComponent<PositionComponent>({200, 100});
    player.AddComponent<SizeComponent>({200, 200});
    player.AddComponent<QTImageComponent>({":/player/images/look/right.png"});
    player.AddComponent<TextComponent>({"Player.", Color::Red()});
    player.AddComponent<OnKeyboardEventComponent>({[position](KeyboardEvent* e) {
        if (e->key() == KeyboardEvent::Key::Left) _Log_("Left");
        else if (e->key() == KeyboardEvent::Key::Right) _Log_("Right");
        else if (e->key() == KeyboardEvent::Key::Up) _Log_("Up");
        else if (e->key() == KeyboardEvent::Key::Down) _Log_("Down");
        if (e->key() == KeyboardEvent::Key::Left) position->SetX(position->x() - 10);
        else if (e->key() == KeyboardEvent::Key::Right) position->SetX(position->x() + 10);
        else if (e->key() == KeyboardEvent::Key::Up) position->SetY(position->y() - 10);
        else if (e->key() == KeyboardEvent::Key::Down) position->SetY(position->y() + 10);
        _Log_("Player position x: {}, y: {}", position->x(), position->y());
    }});
}

void AddTextLabel(Game& game) {
    auto label = game.Entities().CreateEntity();
    label.AddComponent<PositionComponent>({100, 100});
    label.AddComponent<RectangleComponent>({
        {100, 100}
    });
    label.AddComponent<TextComponent>({"Hello from the entity."});
}

// We'll add a click handler component to this one or figure out how to use Qt input event --->
// CommandSystem
void AddGraphicsRectButton(Game& game) {
    auto button = game.Entities().CreateEntity();
    button.AddComponent<RectangleComponent>({
        {100, 100},
        Color::Yellow()
    });
    button.AddComponent<TextComponent>({"Click me!", Color::Black()});
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
    window.setWindowTitle("Side Scroller");
    auto* layout = new QVBoxLayout();
    window.setLayout(layout);
    GraphicsViewWithFPS        view(game.Events());
    EventEmittingGraphicsScene scene(game.Events());
    scene.setSceneRect(0, 0, 800, 600);
    view.setScene(&scene);
    layout->addWidget(&view.FPSLabel());
    loopIterationLabel = new QLabel();
    layout->addWidget(loopIterationLabel);
    layout->addWidget(&view);
    window.show();
    ///////////////////////

    SetupQtRenderSystem(game, scene);
    game.Systems().AddSystem<CommandSystem>();

    KeyboardInputSystem keyboardInputSystem(game.Entities().GetEntityManager());
    keyboardInputSystem.RegisterListener(game.Events());
    game.Systems().AddSystem(&keyboardInputSystem);

    AddPlayer(game);

    game.Events().AddListener<KeyboardEvent>([](KeyboardEvent* event) { qDebug() << "Key"; });
    game.Events().AddListener<MouseClickEvent>([](MouseClickEvent* event) { qDebug() << "Mouse"; });

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);

    ///////////////////////
    return app.exec();
}
