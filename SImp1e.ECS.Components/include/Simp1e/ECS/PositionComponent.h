#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/Position.h>

#include "ComponentTypes.h"

namespace Simp1e::ECS {
    class PositionComponent {
        Position _position;

    public:
        SIMP1E_ECS_COMPONENT(ComponentTypes::Position)

        PositionComponent() = default;
        PositionComponent(sreal x, sreal y) : _position(x, y) {}
        PositionComponent(const Position& position) : _position(position) {}

        Position& GetPosition() { return _position; }
        void      SetPosition(const Position& position) { _position = position; }
    };
}
