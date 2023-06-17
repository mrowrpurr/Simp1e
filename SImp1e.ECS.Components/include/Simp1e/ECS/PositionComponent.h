#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Position.h>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class PositionComponent : public ComponentBase {
        Position _position;

    public:
        SIMP1E_ECS_COMPONENT("Position")

        PositionComponent() = default;
        PositionComponent(sreal x, sreal y) : _position(x, y) {}
        PositionComponent(const Position& position) : _position(position) {}

        virtual Position const GetPosition() { return _position; }

        virtual void SetPosition(const Position& position) {
            _position = position;
            SetDirty();
        }
    };
}
