#include <Simp1e/QT/_Log_.h>
//

// TODO rename all of my Qt* files and classes to QT* so it doesn't collide with Qt's classes

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
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
#include <QGraphicsView>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>
#include <QWidget>

using namespace Simp1e;
using namespace Simp1e::ECS;

class GraphicsViewWithFPS : public QGraphicsView {
    QLabel        fpsLabel;
    QElapsedTimer fpsTimer;
    int           frames = 0;

public:
    GraphicsViewWithFPS() { fpsTimer.start(); }

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
};

void SetupQtRenderSystem(Game& game, QGraphicsScene& scene) {
    auto* qtRenderSystem = new QtRenderSystem(game, scene);
    qtRenderSystem->AddVisualComponentType<VisibleComponent>();
    //
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
    GraphicsViewWithFPS view;
    QGraphicsScene      scene(0, 0, 800, 600);
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

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);

    ///////////////////////
    return app.exec();
}
