#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Position.h>

namespace Simp1e {

    struct IPositionComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Position")

        enum class Fields : int {
            X = 1 << 0,
            Y = 1 << 1,
            Z = 1 << 2,
        };

        virtual Position GetPosition() const = 0;

        virtual sreal x() const = 0;
        virtual sreal y() const = 0;
        virtual sreal z() const = 0;

        virtual void SetX(sreal x)                         = 0;
        virtual void SetY(sreal y)                         = 0;
        virtual void SetZ(sreal z)                         = 0;
        virtual void SetPosition(const Position& position) = 0;
    };
}