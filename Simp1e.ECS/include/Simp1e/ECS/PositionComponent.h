#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/Position.h>

#include "DirtyTrackingComponent.h"

namespace Simp1e::ECS {
    class PositionComponent : public DirtyTrackingComponent {
        Position _position;
        Position _previousPosition;

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
        virtual void SetX(sreal x) {
            auto& xRef = _position.x();
            if (xRef != x) {
                xRef = x;
                SetDirty();
            }
        }
        virtual void SetY(sreal y) {
            auto& yRef = _position.y();
            if (yRef != y) {
                yRef = y;
                SetDirty();
            }
        }

        virtual sreal    x() const { return _position.x(); }
        virtual sreal    y() const { return _position.y(); }
        virtual Position position() const { return _position; }

        virtual Position const previousPosition() { return _previousPosition; }
        virtual void           SetPreviousPosition(const Position& position) { _previousPosition = position; }
        virtual void           StoreLastPosition() { _previousPosition = _position; }
        virtual void           RestoreLastPosition() {
            _position = _previousPosition;
            SetDirty();
        }
    };
}
