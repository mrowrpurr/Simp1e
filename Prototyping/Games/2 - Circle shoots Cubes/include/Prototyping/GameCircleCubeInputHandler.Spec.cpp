#define spec_name GameCircleCubeInputHandler

#include "GameCircleCubeInputHandler.h"

#include "GameSpecHelper.h"

Describe("GameCircleCubeInputHandler") {
    it("moves the circle to a left-clicked cell", []() {
        GameCircleCube             game{5, 5};
        GameCircleCubeInputHandler inputHandler{game};

        game.MoveCircleTo({1, 1});
        AssertThat(game.GetCircleTile(), Equals(Coordinate{1, 1}));

        inputHandler.OnLeftClick({2, 2});
        AssertThat(game.GetCircleTile(), Equals(Coordinate{2, 2}));
    });

    it("places a cube in right-clicked tiles", []() {
        GameCircleCube             game{5, 5};
        GameCircleCubeInputHandler inputHandler{game};

        AssertThat(game.GetCubeTiles().size(), Equals(0));

        inputHandler.OnRightClick({2, 2});
        AssertThat(game.GetCubeTiles().size(), Equals(1));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{2, 2}));

        inputHandler.OnRightClick({1, 1});
        AssertThat(game.GetCubeTiles().size(), Equals(2));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{1, 1}));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{2, 2}));
    });
}