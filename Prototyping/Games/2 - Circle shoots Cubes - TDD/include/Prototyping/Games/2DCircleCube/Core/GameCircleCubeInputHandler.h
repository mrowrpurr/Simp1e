#pragma once

#include "GameCircleCube.h"

namespace Simp1e::Games::CircleCube {

    class GameCircleCubeInputHandler {
        GameCircleCube& _game;

    public:
        GameCircleCubeInputHandler(GameCircleCube& game) : _game(game) {}

        void OnLeftClick(const Coordinate& cell) { _game.MoveCircleTo(cell); }
        void OnRightClick(const Coordinate& cell) { _game.AddCubeTo(cell); }
    };
}
