#pragma once

#include "GameCircleCube.h"

namespace Prototyping {

    class GameCircleCubeInputHandler {
        GameCircleCube& _game;

    public:
        GameCircleCubeInputHandler(GameCircleCube& game) : _game(game) {}

        void OnLeftClick(Coordinate cell) { _game.MoveCircleTo(cell); }
    };
}
