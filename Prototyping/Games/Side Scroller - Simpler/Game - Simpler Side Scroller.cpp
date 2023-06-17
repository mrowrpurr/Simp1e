#include <Simp1e/Color.h>
#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/CommandTypeMacro.h>
#include <Simp1e/ECS/EventTypeMacro.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/RectangleComponent.h>

#include <QApplication>
#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>

using namespace Simp1e;
using namespace Simp1e::ECS;

// class InputSystem {
// public:
//     static SystemType GetSystemType() { return "InputSystem"; }
//     void              Update() { qDebug() << "InputSystem::Update()"; }
// };

class OnClickEvent {
public:
    SIMP1E_ECS_EVENT("OnClickEvent")
    std::string someData;
};

class SystemOne {
    Game& _game;

public:
    SystemOne(Game& game) : _game(game) {}
    SIMP1E_ECS_SYSTEM("SystemOne")
    void Update() {
        qDebug() << "SystemOne::Update()";
        qDebug() << "The game has " << _game.Systems().GetSystemCount() << " systems.";
        _game.Events().SendEvent<OnClickEvent>({"EVENT FROM SYSTEM ONE"});
    }
};

class SystemTwo {
public:
    SIMP1E_ECS_SYSTEM("SystemTwo")
    void Update() {
        qDebug() << "SystemTwo::Update()";
        // How can I emit an event here?
    }
};

class CommandOne {
    std::string text;

public:
    SIMP1E_ECS_COMMAND("CommandOne")
    CommandOne(const std::string& text) : text(text) {}
    void Execute() { qDebug() << "CommandOne::Execute() - " << text.c_str(); }
};

int main(int argc, char* argv[]) {
    Game game;
    game.Systems().AddSystem<SystemOne>(game);
    game.Systems().AddSystem<SystemTwo>();
    game.Systems().AddSystem<CommandSystem>();
    game.Events().AddListener<OnClickEvent>([&game](OnClickEvent* event) {
        qDebug() << "OnClickEvent data: " << event->someData.c_str();
        game.Systems().GetSystem<CommandSystem>()->AddCommand<CommandOne>(
            {"Made command from click"}
        );
    });

    QApplication   app(argc, argv);
    QGraphicsView  window;
    QGraphicsScene scene(0, 0, 800, 600);
    window.setScene(&scene);

    qDebug() << "Run app";

    //
    game.Update();

    game.Systems().DisableSystem<SystemOne>();

    game.Update();

    game.Events().SendEvent<OnClickEvent>({"Some data"});

    game.Update();

    window.setWindowTitle("Side Scroller");
    window.show();
    return app.exec();
}

// int main(int argc, char* argv[]) {
//     QApplication   app(argc, argv);
//     QGraphicsView  window;
//     QGraphicsScene scene(0, 0, 800, 600);
//     window.setScene(&scene);

//     qDebug() << "Run app";

//     ManagedEntityManager entityManager;

//     // Add one rectangle entity with a position and a fill color
//     auto entity = entityManager.CreateEntity();
//     entity.AddComponent<PositionComponent>({100, 100});

//     auto rectComponent = new QtRectangleComponent({300, 400, 50, 200}, Color::Magenta());
//     entity.AddComponent(rectComponent);

//     // Systems...
//     DrawSomethingSystem drawSomethingSystem(scene);

//     // Main loop
//     // while (true) {
//     // Update systems
//     drawSomethingSystem.Update(entityManager);

//     // Update window
//     // window.update();
//     // app.processEvents();
//     // }

//     window.setWindowTitle("Side Scroller");
//     window.show();
//     return app.exec();
// }

// QColor ToQColor(const Color& color) {
//     return QColor(color.red(), color.green(), color.blue(), color.alpha());
// }

// class QtRectangleComponent : public RectangleComponent {
//     bool              _initialized = false;
//     QGraphicsRectItem _rectItem;

//     // Delete copy constructor - not possible with std::any
//     QtRectangleComponent(const QtRectangleComponent&) = delete;

// public:
//     SIMP1E_ECS_COMPONENT("QtRectangleComponent")

//     QtRectangleComponent(const Rectangle& rectangle)
//         : RectangleComponent(rectangle),
//           _rectItem(rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height()) {}
//     QtRectangleComponent(const Rectangle& rectangle, const Color& fillColor)
//         : QtRectangleComponent(rectangle) {
//         SetFillColor(fillColor);
//     }
//     QtRectangleComponent(const Rectangle& rectangle, const std::optional<Color>& fillColor)
//         : QtRectangleComponent(rectangle) {
//         if (fillColor.has_value()) SetFillColor(fillColor.value());
//     }

//     QGraphicsRectItem& GetRectItem() { return _rectItem; }

//     void SetRectangle(const Rectangle& rectangle) override {
//         RectangleComponent::SetRectangle(rectangle);
//         _rectItem.setRect(rectangle.x(), rectangle.y(), rectangle.width(), rectangle.height());
//     }

//     void SetFillColor(const Color& fillColor) override {
//         RectangleComponent::SetFillColor(fillColor);
//         _rectItem.setBrush(ToQColor(fillColor));
//     }

//     void Initialize(QGraphicsScene& scene) {
//         if (!_initialized) {
//             scene.addItem(&_rectItem);
//             _initialized = true;
//         }
//     }
// };

// class DrawSomethingSystem {
//     QGraphicsScene& _scene;

// public:
//     DrawSomethingSystem(QGraphicsScene& scene) : _scene(scene) {}

//     void Update(ManagedEntityManager& entityManager) {
//         for (auto& [entity, component] : entityManager.GetComponents<QtRectangleComponent>()) {
//             auto rectangleComponent = static_cast<QtRectangleComponent*>(component.get());
//             rectangleComponent->Initialize(_scene);
//         }
//     }
// };