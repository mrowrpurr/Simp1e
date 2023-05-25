#define spec_name QtGameCircleCubeScene

#include "QtGameCircleCubeScene.h"

#include <QApplication>
#include <QGraphicsView>

#include "GameSpecHelper.h"

using namespace Prototyping;

Describe("QtGameCircleCubeScene") {
    it("can specify cell width and height", []() {
        SetupQtApp app;

        GameCircleCube game{5, 10};

        QtGameCircleCubeScene scene(game);
        AssertThat(scene.GetCellWidth(), Equals(0));
        AssertThat(scene.GetCellHeight(), Equals(0));

        QtGameCircleCubeScene scene2(game, {.cellWidth = 5, .cellHeight = 8});
        AssertThat(scene2.GetCellWidth(), Equals(5));
        AssertThat(scene2.GetCellHeight(), Equals(8));
    });

    it("has a size that fits the map", []() {
        SetupQtApp app;

        GameCircleCube game{5, 10};
        QGraphicsView  view;

        QtGameCircleCubeSceneParams sceneParams = {.cellWidth = 5, .cellHeight = 8};
        QtGameCircleCubeScene       scene(game, sceneParams);

        view.setScene(&scene);
        view.show();

        AssertThat(
            scene.width(), Is().GreaterThanOrEqualTo(game.GetColumnCount() * sceneParams.cellWidth)
        );
        AssertThat(
            scene.height(), Is().GreaterThanOrEqualTo(game.GetRowCount() * sceneParams.cellHeight)
        );
    });

    it("has a circle on its scene", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.cellWidth = 6, .cellHeight = 10};
        QtGameCircleCubeScene       scene(game, sceneParams);

        game.MoveCircleTo({1, 2});
        auto* circle = scene.GetCircle();
        AssertThat(circle, Is().Not().Null());
        AssertThat(
            circle->pos().x(), Equals(
                                   (1 * sceneParams.cellWidth) + (sceneParams.cellWidth / 2) -
                                   (sceneParams.circleSize / 2)
                               )
        );
        AssertThat(
            circle->pos().y(), Equals(
                                   (2 * sceneParams.cellHeight) + (sceneParams.cellHeight / 2) -
                                   (sceneParams.circleSize / 2)
                               )
        );
    });

    // it("can get coordinate of position", []() {

    // });
}