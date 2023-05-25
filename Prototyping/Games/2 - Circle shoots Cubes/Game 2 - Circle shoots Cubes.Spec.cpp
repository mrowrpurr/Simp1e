#include "Prototyping/GameCircleCube.h"
#include "SpecHelper.h"

using namespace Prototyping;

Describe("Circle Shoots Cubes Game") {
    it("has a circle at a specific position", []() {
        GameCircleCube game;
        game.MoveCircleTo(5, 8);
        AssertThat(game.GetCirclePosition().x, Equals(5));
        AssertThat(game.GetCirclePosition().y, Equals(8));
    });
    // it("can have cubes", [](){});
}