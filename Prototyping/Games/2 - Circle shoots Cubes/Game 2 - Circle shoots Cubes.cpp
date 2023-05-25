#include <QApplication>
#include <QGraphicsView>

#include "Prototyping/GameCircleCube.h"
#include "Prototyping/QtGameCircleCubeScene.h"

using namespace Prototyping;

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    GameCircleCube game{10, 8};
    QGraphicsView  view;

    QtGameCircleCubeSceneParams sceneParams = {
        .cellWidth       = 50,
        .cellHeight      = 75,
        .circleSize      = 25,
        .renderGridLines = true,
    };
    QtGameCircleCubeScene scene(game, sceneParams);

    view.setScene(&scene);
    view.show();

    return app.exec();
}
