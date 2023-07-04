#pragma once

#include <Simp1e/DirtyTrackingComponentBase.h>

#include "IPositionComponent.h"

namespace Simp1e {

    class PositionComponent : public IPositionComponent {
        Position _position;

    public:
        PositionComponent() = default;
        PositionComponent(const Position& position) : _position(position) {}

        sreal x() const override { return _position.x(); }
        sreal y() const override { return _position.y(); }
        sreal z() const override { return _position.z(); }

        void SetX(sreal x) override {
            _position.SetX(x);
            SetDirtyFlag(Fields::X);
        }
        void SetY(sreal y) override {
            _position.SetY(y);
            SetDirtyFlag(Fields::Y);
        }
        void SetZ(sreal z) override {
            _position.SetZ(z);
            SetDirtyFlag(Fields::Z);
        }
        void SetPosition(const Position& position) override {
            _position = position;
            SetDirtyFlags(Fields::X, Fields::Y, Fields::Z);
        }
    };
}