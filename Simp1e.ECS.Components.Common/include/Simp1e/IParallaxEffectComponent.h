#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Position.h>
#include <Simp1e/sreal.h>

namespace Simp1e {

    struct IParallaxEffectComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("IParallaxEffect")

        enum class Fields : int {
            LayersChanged             = 1 << 0,
            TargetPerspectivePosition = 1 << 1,
        };

        virtual int  AddLayer(const char* path, sreal speed = 1.0, sreal scale = 1.0f) = 0;
        virtual void EnableLayer(int index)                                            = 0;
        virtual void DisableLayer(int index)                                           = 0;
        virtual void RemoveLayer(int index)                                            = 0;
        virtual bool IsLayerEnabled(int index) const                                   = 0;

        virtual void     SetTargetPerspectivePosition(const Position& position) = 0;
        virtual Position GetTargetPerspectivePosition() const                   = 0;
    };
}