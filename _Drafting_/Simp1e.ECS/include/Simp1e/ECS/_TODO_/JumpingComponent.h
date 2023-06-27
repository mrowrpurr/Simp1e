#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/sreal.h>

namespace Simp1e::ECS {
    class JumpingComponent {
        sreal _jumpHeight        = 0.0;
        sreal _currentJumpHeight = 0.0;
        sreal _jumpSpeed         = 0.0;

    public:
        SIMP1E_ECS_COMPONENT("Jumping")

        JumpingComponent() = default;
        JumpingComponent(sreal jumpHeight, sreal jumpSpeed) : _jumpHeight(jumpHeight), _jumpSpeed(jumpSpeed) {}

        virtual sreal JumpHeight() const { return _jumpHeight; }
        virtual sreal CurrentJumpHeight() const { return _currentJumpHeight; }
        virtual sreal JumpSpeed() const { return _jumpSpeed; }

        virtual void SetCurrentJumpHeight(sreal currentJumpHeight) { _currentJumpHeight = currentJumpHeight; }
    };
}
