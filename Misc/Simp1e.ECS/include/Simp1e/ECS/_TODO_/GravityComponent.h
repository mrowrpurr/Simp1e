#pragma once

#include <Simp1e/ECS/ComponentTypeMacro.h>
#include <Simp1e/sreal.h>

#include "DirtyTrackingComponent.h"

namespace Simp1e::ECS {
    class GravityComponent : public DirtyTrackingComponent {
        sreal _gravityFactor          = 1.0;
        sreal _terminalVelocityFactor = 1.0;
        sreal _currentFallSpeed       = 0.0;
        bool  _isGrounded             = false;

    public:
        struct Params {
            sreal gravityFactor          = 1.0;
            sreal terminalVelocityFactor = 1.0;
            sreal currentFallSpeed       = 0.0;
            bool  isGrounded             = false;
        };

        SIMP1E_ECS_COMPONENT("Gravity")

        GravityComponent(
            sreal gravityFactor = 1.0, sreal terminalVelocityFactor = 1.0, sreal currentFallSpeed = 0.0,
            bool isGrounded = false
        )
            : _gravityFactor(gravityFactor),
              _terminalVelocityFactor(terminalVelocityFactor),
              _currentFallSpeed(currentFallSpeed),
              _isGrounded(isGrounded) {}
        GravityComponent(const Params& params)
            : _gravityFactor(params.gravityFactor),
              _terminalVelocityFactor(params.terminalVelocityFactor),
              _currentFallSpeed(params.currentFallSpeed),
              _isGrounded(params.isGrounded) {}

        virtual sreal GravityFactor() const { return _gravityFactor; }
        virtual sreal TerminalVelocityFactor() const { return _terminalVelocityFactor; }
        virtual sreal CurrentFallSpeed() const { return _currentFallSpeed; }
        virtual bool  IsGrounded() const { return _isGrounded; }

        virtual void SetGravityFactor(sreal gravityFactor) {
            if (_gravityFactor != gravityFactor) {
                _gravityFactor = gravityFactor;
                SetDirty();
            }
        }
        virtual void SetTerminalVelocityFactor(sreal terminalVelocityFactor) {
            if (_terminalVelocityFactor != terminalVelocityFactor) {
                _terminalVelocityFactor = terminalVelocityFactor;
                SetDirty();
            }
        }
        virtual void SetCurrentFallSpeed(sreal currentFallSpeed) {
            if (_currentFallSpeed != currentFallSpeed) {
                _currentFallSpeed = currentFallSpeed;
                SetDirty();
            }
        }
        virtual void SetIsGrounded(bool isGrounded = true) {
            if (_isGrounded != isGrounded) {
                _isGrounded = isGrounded;
                SetDirty();
            }
        }
    };
}
