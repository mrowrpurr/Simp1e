#include <Simp1e/QT/_Log_.h>
//

// TODO rename all of my Qt* files and classes to QT* so it doesn't collide with Qt's classes

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/EventTypeMacro.h>
#include <Simp1e/ECS/Game.h>
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

using namespace Simp1e;
using namespace Simp1e::ECS;

class MouseClickEvent {
public:
    enum class Button { Left, Right, Middle };

private:
    sreal  _x;
    sreal  _y;
    Button _button;
    bool   _pressed;

public:
    SIMP1E_ECS_EVENT("Mouse")

    MouseClickEvent(sreal x, sreal y, Button button = Button::Left, bool pressed = true)
        : _x(x), _y(y), _button(button), _pressed(pressed) {}

    virtual ~MouseClickEvent() = default;

    virtual sreal  X() const { return _x; }
    virtual sreal  Y() const { return _y; }
    virtual Button button() const { return _button; }
    virtual bool   leftButton() const { return _button == Button::Left; }
    virtual bool   rightButton() const { return _button == Button::Right; }
    virtual bool   middleButton() const { return _button == Button::Middle; }
    virtual bool   pressed() const { return _pressed; }
};

MouseClickEvent::Button FromQtMouseButton(Qt::MouseButton button) {
    switch (button) {
        case Qt::MouseButton::LeftButton:
            return MouseClickEvent::Button::Left;
        case Qt::MouseButton::RightButton:
            return MouseClickEvent::Button::Right;
        case Qt::MouseButton::MiddleButton:
            return MouseClickEvent::Button::Middle;
        default:
            return MouseClickEvent::Button::Left;
    }
}

MouseClickEvent ToMouseClickEvent(QGraphicsSceneMouseEvent* event, bool pressed = true) {
    return MouseClickEvent(
        event->scenePos().x(), event->scenePos().y(), FromQtMouseButton(event->button()), pressed
    );
}

class KeyboardEvent {
public:
    enum class Key {
        Unknown,
        Backspace,
        Tab,
        Enter,
        Shift,
        Ctrl,
        Alt,
        Pause,
        CapsLock,
        Escape,
        Space,
        PageUp,
        PageDown,
        End,
        Home,
        Left,
        Up,
        Right,
        Down,
        PrintScreen,
        Insert,
        Delete,
        Key0,
        Key1,
        Key2,
        Key3,
        Key4,
        Key5,
        Key6,
        Key7,
        Key8,
        Key9,
        KeyA,
        KeyB,
        KeyC,
        KeyD,
        KeyE,
        KeyF,
        KeyG,
        KeyH,
        KeyI,
        KeyJ,
        KeyK,
        KeyL,
        KeyM,
        KeyN,
        KeyO,
        KeyP,
        KeyQ,
        KeyR,
        KeyS,
        KeyT,
        KeyU,
        KeyV,
        KeyW,
        KeyX,
        KeyY,
        KeyZ,
        LeftSuper,
        RightSuper,
        Menu,
        Numpad0,
        Numpad1,
        Numpad2,
        Numpad3,
        Numpad4,
        Numpad5,
        Numpad6,
        Numpad7,
        Numpad8,
        Numpad9,
        NumpadMultiply,
        NumpadAdd,
        NumpadSubtract,
        NumpadDecimal,
        NumpadDivide,
        F1,
        F2,
        F3,
        F4,
        F5,
        F6,
        F7,
        F8,
        F9,
        F10,
        F11,
        F12,
        NumLock,
        ScrollLock,
        LeftShift,
        RightShift,
        LeftCtrl,
        RightCtrl,
        LeftAlt,
        RightAlt,
        Semicolon,
        Apostrophe,
        Hyphen,
        Equal,
        Comma,
        Minus,
        Period,
        Slash,
        Backquote,
        LeftBracket,
        Backslash,
        RightBracket
    };

private:
    Key  _key;
    bool _pressed;
    bool _repeated;

public:
    SIMP1E_ECS_EVENT("Keyboard")

    KeyboardEvent(Key key, bool pressed = true, bool repeated = false)
        : _key(key), _pressed(pressed), _repeated(repeated) {}

    virtual ~KeyboardEvent() = default;

    virtual Key  key() const { return _key; }
    virtual bool pressed() const { return _pressed; }
    virtual bool released() const { return !_pressed; }
    virtual bool repeated() const { return _repeated; }
};

KeyboardEvent::Key FromQtKey(int key) {
    switch (key) {
        case Qt::Key::Key_0:
            return KeyboardEvent::Key::Key0;
        case Qt::Key::Key_1:
            return KeyboardEvent::Key::Key1;
        case Qt::Key::Key_2:
            return KeyboardEvent::Key::Key2;
        case Qt::Key::Key_3:
            return KeyboardEvent::Key::Key3;
        case Qt::Key::Key_4:
            return KeyboardEvent::Key::Key4;
        case Qt::Key::Key_5:
            return KeyboardEvent::Key::Key5;
        case Qt::Key::Key_6:
            return KeyboardEvent::Key::Key6;
        case Qt::Key::Key_7:
            return KeyboardEvent::Key::Key7;
        case Qt::Key::Key_8:
            return KeyboardEvent::Key::Key8;
        case Qt::Key::Key_9:
            return KeyboardEvent::Key::Key9;
        case Qt::Key::Key_A:
            return KeyboardEvent::Key::KeyA;
        case Qt::Key::Key_B:
            return KeyboardEvent::Key::KeyB;
        case Qt::Key::Key_C:
            return KeyboardEvent::Key::KeyC;
        case Qt::Key::Key_D:
            return KeyboardEvent::Key::KeyD;
        case Qt::Key::Key_E:
            return KeyboardEvent::Key::KeyE;
        case Qt::Key::Key_F:
            return KeyboardEvent::Key::KeyF;
        case Qt::Key::Key_G:
            return KeyboardEvent::Key::KeyG;
        case Qt::Key::Key_H:
            return KeyboardEvent::Key::KeyH;
        case Qt::Key::Key_I:
            return KeyboardEvent::Key::KeyI;
        case Qt::Key::Key_J:
            return KeyboardEvent::Key::KeyJ;
        case Qt::Key::Key_K:
            return KeyboardEvent::Key::KeyK;
        case Qt::Key::Key_L:
            return KeyboardEvent::Key::KeyL;
        case Qt::Key::Key_M:
            return KeyboardEvent::Key::KeyM;
        case Qt::Key::Key_N:
            return KeyboardEvent::Key::KeyN;
        case Qt::Key::Key_O:
            return KeyboardEvent::Key::KeyO;
        case Qt::Key::Key_P:
            return KeyboardEvent::Key::KeyP;
        case Qt::Key::Key_Q:
            return KeyboardEvent::Key::KeyQ;
        case Qt::Key::Key_R:
            return KeyboardEvent::Key::KeyR;
        case Qt::Key::Key_S:
            return KeyboardEvent::Key::KeyS;
        case Qt::Key::Key_T:
            return KeyboardEvent::Key::KeyT;
        case Qt::Key::Key_U:
            return KeyboardEvent::Key::KeyU;
        case Qt::Key::Key_V:
            return KeyboardEvent::Key::KeyV;
        case Qt::Key::Key_W:
            return KeyboardEvent::Key::KeyW;
        case Qt::Key::Key_X:
            return KeyboardEvent::Key::KeyX;
        case Qt::Key::Key_Y:
            return KeyboardEvent::Key::KeyY;
        case Qt::Key::Key_Z:
            return KeyboardEvent::Key::KeyZ;
        case Qt::Key::Key_Super_L:
            return KeyboardEvent::Key::LeftSuper;
        case Qt::Key::Key_Super_R:
            return KeyboardEvent::Key::RightSuper;
        case Qt::Key::Key_Menu:
            return KeyboardEvent::Key::Menu;
        case Qt::Key::Key_F1:
            return KeyboardEvent::Key::F1;
        case Qt::Key::Key_F2:
            return KeyboardEvent::Key::F2;
        case Qt::Key::Key_F3:
            return KeyboardEvent::Key::F3;
        case Qt::Key::Key_F4:
            return KeyboardEvent::Key::F4;
        case Qt::Key::Key_F5:
            return KeyboardEvent::Key::F5;
        case Qt::Key::Key_F6:
            return KeyboardEvent::Key::F6;
        case Qt::Key::Key_F7:
            return KeyboardEvent::Key::F7;
        case Qt::Key::Key_F8:
            return KeyboardEvent::Key::F8;
        case Qt::Key::Key_F9:
            return KeyboardEvent::Key::F9;
        case Qt::Key::Key_F10:
            return KeyboardEvent::Key::F10;
        case Qt::Key::Key_F11:
            return KeyboardEvent::Key::F11;
        case Qt::Key::Key_F12:
            return KeyboardEvent::Key::F12;
        case Qt::Key::Key_AsciiTilde:
            return KeyboardEvent::Key::Backquote;
        case Qt::Key::Key_Minus:
            return KeyboardEvent::Key::Hyphen;
        case Qt::Key::Key_Equal:
            return KeyboardEvent::Key::Equal;
        case Qt::Key::Key_BracketLeft:
            return KeyboardEvent::Key::LeftBracket;
        case Qt::Key::Key_BracketRight:
            return KeyboardEvent::Key::RightBracket;
        case Qt::Key::Key_Backslash:
            return KeyboardEvent::Key::Backslash;
        case Qt::Key::Key_Semicolon:
            return KeyboardEvent::Key::Semicolon;
        case Qt::Key::Key_Apostrophe:
            return KeyboardEvent::Key::Apostrophe;
        case Qt::Key::Key_Comma:
            return KeyboardEvent::Key::Comma;
        case Qt::Key::Key_Period:
            return KeyboardEvent::Key::Period;
        case Qt::Key::Key_Slash:
            return KeyboardEvent::Key::Slash;
        case Qt::Key::Key_NumLock:
            return KeyboardEvent::Key::NumLock;
        case Qt::Key::Key_ScrollLock:
            return KeyboardEvent::Key::ScrollLock;
        default:
            return KeyboardEvent::Key::Unknown;
    }
}

KeyboardEvent ToKeyboardEvent(QKeyEvent* event, bool pressed = true) {
    return KeyboardEvent(FromQtKey(event->key()), pressed, event->isAutoRepeat());
}

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

void AddPlayer(Game& game) {
    auto player = game.Entities().CreateEntity();
    player.AddComponent<PositionComponent>({200, 100});
    player.AddComponent<SizeComponent>({200, 200});
    player.AddComponent<QTImageComponent>({":/player/images/look/right.png"});
    player.AddComponent<TextComponent>({"Player.", Color::Red()});
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

    AddPlayer(game);

    game.Events().AddListener<KeyboardEvent>([](KeyboardEvent* event) { qDebug() << "Key"; });
    game.Events().AddListener<MouseClickEvent>([](MouseClickEvent* event) { qDebug() << "Mouse"; });

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);

    ///////////////////////
    return app.exec();
}
