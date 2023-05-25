#define spec_name GameCircleCubeInputHandler

#include "GameSpecHelper.h"

Describe("GameCircleCubeInputHandler") {
    it("moves the circle to a left-clicked cell", []() {
        GameCircleCube             game{5, 5};
        GameCircleCubeInputHandler inputHandler{game};

        game.MoveCircleTo({1, 1});
        AssertThat(game.GetCirclePosition(), Equals(Coordinate{1, 1}));

        inputHandler.OnLeftClick({2, 2});
        AssertThat(game.GetCirclePosition(), Equals(Coordinate{2, 2}));
    });
}