#define spec_name QtGameCircleCubeScene

#include "QtGameCircleCubeScene.h"

#include <QApplication>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>

#include "CircleCubeSpecHelper.h"

using namespace Simp1e;

Describe("QtGameCircleCubeScene") {
    it("can specify tile width and height", []() {
        SetupQtApp app;

        GameCircleCube game{5, 10};

        QtGameCircleCubeScene scene(game);
        AssertThat(scene.GetCellWidth(), Equals(0));
        AssertThat(scene.GetCellHeight(), Equals(0));

        QtGameCircleCubeScene scene2(game, {.tileWidth = 5, .tileHeight = 8});
        AssertThat(scene2.GetCellWidth(), Equals(5));
        AssertThat(scene2.GetCellHeight(), Equals(8));
    });

    it("has a size that fits the map", []() {
        SetupQtApp app;

        GameCircleCube game{5, 10};
        QGraphicsView  view;

        QtGameCircleCubeSceneParams sceneParams = {.tileWidth = 5, .tileHeight = 8};
        QtGameCircleCubeScene       scene(game, sceneParams);

        view.setScene(&scene);
        view.show();

        AssertThat(
            scene.width(), Is().GreaterThanOrEqualTo(game.GetColumnCount() * sceneParams.tileWidth)
        );
        AssertThat(
            scene.height(), Is().GreaterThanOrEqualTo(game.GetRowCount() * sceneParams.tileHeight)
        );
    });

    it("has a circle on its scene", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.tileWidth = 6, .tileHeight = 10};
        QtGameCircleCubeScene       scene(game, sceneParams);

        game.MoveCircleTo({1, 2});
        auto* circle = scene.GetCircle();
        AssertThat(circle, Is().Not().Null());
        AssertThat(
            circle->pos().x(), Equals(
                                   (1 * sceneParams.tileWidth) + (sceneParams.tileWidth / 2) -
                                   (sceneParams.circleSize / 2)
                               )
        );
        AssertThat(
            circle->pos().y(), Equals(
                                   (2 * sceneParams.tileHeight) + (sceneParams.tileHeight / 2) -
                                   (sceneParams.circleSize / 2)
                               )
        );
    });

    it("can have cubes on its scene", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.tileWidth = 6, .tileHeight = 10, .cubeSize = 5};
        QtGameCircleCubeScene       scene(game, sceneParams);

        AssertThat(scene.GetCubes().size(), Equals(0));

        game.AddCubeTo({1, 2});
        AssertThat(scene.GetCubes().size(), Equals(1));

        auto  expectedCubeBoundingBox = scene.TileToPosition({1, 2});
        auto* cube                    = scene.GetCubes().front();
        AssertThat(
            expectedCubeBoundingBox.Contains(
                {static_cast<uint32_t>(cube->pos().x()), static_cast<uint32_t>(cube->pos().y())}
            ),
            IsTrue()
        );
        AssertThat(
            cube->pos().x(),
            Equals(
                1 * sceneParams.tileWidth + (sceneParams.tileWidth / 2) - (sceneParams.cubeSize / 2)
            )
        );
        AssertThat(
            cube->pos().y(), Equals(
                                 2 * sceneParams.tileHeight + (sceneParams.tileHeight / 2) -
                                 (sceneParams.cubeSize / 2)
                             )
        );

        game.AddCubeTo({3, 4});
        AssertThat(scene.GetCubes().size(), Equals(2));

        bool found                    = false;
        auto expectedCubeBoundingBox2 = scene.TileToPosition({3, 4});
        for (auto* cube : scene.GetCubes()) {
            if (expectedCubeBoundingBox2.Contains(
                    {static_cast<uint32_t>(cube->pos().x()), static_cast<uint32_t>(cube->pos().y())}
                )) {
                found = true;
                break;
            }
        }
        AssertThat(found, IsTrue());
    });

    it("can get tile from a Qt position", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.tileWidth = 6, .tileHeight = 10};
        QtGameCircleCubeScene       scene(game, sceneParams);
        game.MoveCircleTo({1, 2});
        auto* circle = scene.GetCircle();

        auto coordinate = scene.PositionToTile(circle->pos());
        AssertThat(coordinate, Equals(Coordinate{1, 2}));

        game.MoveCircleTo({3, 4});
        auto coordinate2 = scene.PositionToTile(circle->pos());
        AssertThat(coordinate2, Equals(Coordinate{3, 4}));
    });

    it("can get Qt position (rect) from a tile", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.tileWidth = 6, .tileHeight = 10};
        QtGameCircleCubeScene       scene(game, sceneParams);
        game.MoveCircleTo({1, 2});
        auto* circle = scene.GetCircle();

        auto actual   = scene.TileToPosition({1, 2});
        auto expected = BoundingBox{
            {1 * sceneParams.tileWidth,                         2 * sceneParams.tileHeight},
            {1 * sceneParams.tileWidth + sceneParams.tileWidth,
             2 * sceneParams.tileHeight + sceneParams.tileHeight                          },
        };
        AssertThat(actual.topLeft.x, Equals(expected.topLeft.x));
        AssertThat(actual.topLeft.y, Equals(expected.topLeft.y));
        AssertThat(actual.bottomRight.x, Equals(expected.bottomRight.x));
        AssertThat(actual.bottomRight.y, Equals(expected.bottomRight.y));
    });

    it("left clicking moves the circle to that position", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.tileWidth = 6, .tileHeight = 10};
        QtGameCircleCubeScene       scene(game, sceneParams);
        game.MoveCircleTo({1, 2});
        auto* circle = scene.GetCircle();

        auto positionToClick = scene.TileToPosition({3, 4});

        AssertThat(game.GetCircleTile(), Equals(Coordinate{1, 2}));

        QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
        pressEvent.setButton(Qt::LeftButton);
        pressEvent.setScenePos(QPointF(positionToClick.topLeft.x, positionToClick.topLeft.y)
        );  // Set your coordinates here
        pressEvent.setButtonDownScenePos(
            Qt::LeftButton, QPointF(positionToClick.topLeft.x, positionToClick.topLeft.y)
        );  // Repeat your coordinates here <--- can we get rid of this? TODO
        QApplication::sendEvent(&scene, &pressEvent);

        AssertThat(game.GetCircleTile(), Equals(Coordinate{3, 4}));
    });

    it("right clicking adds cubes to that position (if not already there)", []() {
        SetupQtApp app;

        GameCircleCube              game{5, 10};
        QtGameCircleCubeSceneParams sceneParams{.tileWidth = 6, .tileHeight = 10};
        QtGameCircleCubeScene       scene(game, sceneParams);

        auto positionToClick = scene.TileToPosition({3, 4});
        AssertThat(game.GetCubeTiles().size(), Equals(0));

        QGraphicsSceneMouseEvent pressEvent(QEvent::GraphicsSceneMousePress);
        pressEvent.setButton(Qt::RightButton);
        pressEvent.setScenePos(QPointF(positionToClick.topLeft.x, positionToClick.topLeft.y)
        );  // Set your coordinates here
        pressEvent.setButtonDownScenePos(
            Qt::RightButton, QPointF(positionToClick.topLeft.x, positionToClick.topLeft.y)
        );  // Repeat your coordinates here
        QApplication::sendEvent(&scene, &pressEvent);

        AssertThat(game.GetCubeTiles().size(), Equals(1));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{3, 4}));
    });
}