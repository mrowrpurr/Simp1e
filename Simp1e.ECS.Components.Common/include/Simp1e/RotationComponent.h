#pragma once

#include <_Log_.h>

#include "IRotationComponent.h"

namespace Simp1e {

    class RotationComponent : public IRotationComponent {
        sreal _rotation = 0;

    public:
        RotationComponent() = default;
        RotationComponent(sreal rotation) : _rotation(rotation) {}

        void Rotate(sreal delta) override {
            _rotation += delta;
            _Log_("[RotationComponent] Rotate: {}", _rotation);
            if (_rotation > 360) _rotation -= 360;
            if (_rotation < 0) _rotation += 360;
            SetDirtyFlag(Fields::Rotation);
        }
        sreal GetRotation() const override { return _rotation; }
        void  SetRotation(sreal rotation) override {
            _rotation = rotation;
            SetDirtyFlag(Fields::Rotation);
        }
    };
}