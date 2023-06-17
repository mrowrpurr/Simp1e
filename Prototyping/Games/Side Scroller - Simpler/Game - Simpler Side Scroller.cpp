#include <Simp1e/QT/_Log_.h>
//

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QtPositionComponentUpdateHandler.h>
#include <Simp1e/ECS/QtRenderSystem.h>
#include <Simp1e/ECS/QtTextComponentRenderer.h>
#include <Simp1e/ECS/QtTextComponentUpdateHandler.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/VisibleComponent.h>

#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

using namespace Simp1e;
using namespace Simp1e::ECS;

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("fusion");

    QWidget window;
    window.setWindowTitle("Side Scroller");

    auto* layout = new QVBoxLayout();

    QGraphicsView  view;
    QGraphicsScene scene(0, 0, 800, 600);
    view.setScene(&scene);
    layout->addWidget(&view);

    window.show();
    ///////////////////////

    Game game;
    game.Systems().AddSystem<CommandSystem>();

    auto* qtRenderSystem = new QtRenderSystem(game, scene);
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, QtPositionComponentUpdateHandler>(
    );
    qtRenderSystem->AddComponentRenderer<TextComponent, QtTextComponentRenderer>();
    qtRenderSystem->AddComponentUpdateHandler<TextComponent, QtTextComponentUpdateHandler>();
    game.Systems().AddSystem(qtRenderSystem);

    auto label = game.Entities().CreateEntity();
    label.AddComponent<VisibleComponent>();
    label.AddComponent<RectangleComponent>({
        {100, 100}
    });
    label.AddComponent<TextComponent>({"Hello from the entity."});
    label.AddComponent<PositionComponent>({100, 100});

    qDebug() << "Run app";

    game.Update();

    _Log_("Changing position");
    // label.GetComponent<PositionComponent>()->SetPosition({200, 200});

    _Log_("Updating");
    game.Update();

    auto        labelEntityId = label.GetEntity();
    QPushButton button("Change text");
    QObject::connect(&button, &QPushButton::clicked, [&game, labelEntityId]() {
        _Log_("Changing text");
        game.Entities()
            .GetComponent<TextComponent>(labelEntityId)
            ->SetText("Hello from the entity. Updated.");
        _Log_("Updating");
        game.Update();
    });
    layout->addWidget(&button);

    window.setLayout(layout);

    // _Log_("Changing text");
    // label.GetComponent<TextComponent>()->SetText("Hello from the entity. Updated.");

    // _Log_("Updating");
    // game.Update();

    ///////////////////////

    return app.exec();
}
