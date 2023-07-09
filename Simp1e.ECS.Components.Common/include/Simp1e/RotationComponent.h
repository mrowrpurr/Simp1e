#pragma once

#include "IRotationComponent.h"

namespace Simp1e {

    class RotationComponent : public IRotationComponent {
        sreal _rotation = 0;

    public:
        RotationComponent() = default;
        RotationComponent(sreal rotation) : _rotation(rotation) {}

        sreal GetRotation() const override { return _rotation; }
        void  SetRotation(sreal rotation) override {
            _rotation = rotation;
            SetDirtyFlag(Fields::Rotation);
        }
    };
}