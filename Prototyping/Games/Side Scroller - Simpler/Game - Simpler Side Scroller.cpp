#include <QDebug>
#define _Log_(...) qDebug(__VA_ARGS__)
//

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/VisibleComponent.h>

#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>

using namespace Simp1e;
using namespace Simp1e::ECS;

int main(int argc, char* argv[]) {
    Game game;
    game.Systems().AddSystem<CommandSystem>();

    QApplication app(argc, argv);
    app.setStyle("fusion");
    QGraphicsView  window;
    QGraphicsScene scene(0, 0, 800, 600);
    window.setScene(&scene);

    auto label = game.Entities().CreateEntity();

    label.AddComponent<VisibleComponent>(VisibleComponent{true});
    label.AddComponent<TextComponent>({"Hello from the entity."});
    label.AddComponent<PositionComponent>({100, 100});

    auto* labelGraphicsComponent = new QTGraphicsItemComponent(game, label, &scene);
    label.AddComponent(labelGraphicsComponent);

    qDebug() << "Run app";

    window.setWindowTitle("Side Scroller");
    window.show();
    return app.exec();
}
