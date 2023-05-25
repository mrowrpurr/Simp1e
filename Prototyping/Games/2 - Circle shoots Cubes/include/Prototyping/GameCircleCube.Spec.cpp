#define spec_name GameCircleCube

#include "GameSpecHelper.h"

Describe("GameCircleCube") {
    it("it had a size (rows and columns)", []() {
        GameCircleCube game;
        AssertThat(game.GetRowCount(), Equals(0));
        AssertThat(game.GetColumnCount(), Equals(0));

        GameCircleCube game2(5, 8);
        AssertThat(game2.GetRowCount(), Equals(8));
        AssertThat(game2.GetColumnCount(), Equals(5));
    });

    it("has a circle at a specific position", []() {
        GameCircleCube game;
        AssertThrows(std::runtime_error, game.MoveCircleTo({5, 8}));
        AssertThat(LastException<std::runtime_error>().what(), Contains("out of bounds"));

        GameCircleCube game2(5, 8);
        AssertThrows(std::runtime_error, game2.MoveCircleTo({5, 8}));
        AssertThat(LastException<std::runtime_error>().what(), Contains("out of bounds"));

        GameCircleCube game3(5, 8);
        game3.MoveCircleTo({4, 7});
        AssertThat(game3.GetCirclePosition().y, Equals(7));
        AssertThat(game3.GetCirclePosition().x, Equals(4));
    });
}
