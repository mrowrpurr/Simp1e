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
        GameCircleCube game;  // 0,0
        AssertThrows(std::runtime_error, game.MoveCircleTo({5, 8}));
        AssertThat(LastException<std::runtime_error>().what(), Contains("out of bounds"));

        GameCircleCube game2(5, 8);
        AssertThrows(std::runtime_error, game2.MoveCircleTo({5, 8}));
        AssertThat(LastException<std::runtime_error>().what(), Contains("out of bounds"));

        GameCircleCube game3(5, 8);
        game3.MoveCircleTo({4, 7});
        AssertThat(game3.GetCircleTile().y, Equals(7));
        AssertThat(game3.GetCircleTile().x, Equals(4));
    });

    it("can have multiple cubes (1 per cell)", []() {
        GameCircleCube game{5, 8};
        AssertThat(game.GetCubeTiles().size(), Equals(0));

        AssertThrows(std::runtime_error, game.AddCubeTo({10, 10}));
        AssertThat(LastException<std::runtime_error>().what(), Contains("out of bounds"));

        game.AddCubeTo({4, 7});
        AssertThat(game.GetCubeTiles().size(), Equals(1));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{4, 7}));

        AssertThrows(std::runtime_error, game.AddCubeTo({4, 7}));
        AssertThat(LastException<std::runtime_error>().what(), Contains("already has a cube"));

        game.AddCubeTo({2, 4});
        AssertThat(game.GetCubeTiles().size(), Equals(2));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{4, 7}));
        AssertThat(game.GetCubeTiles(), Contains(Coordinate{2, 4}));
    });
}
