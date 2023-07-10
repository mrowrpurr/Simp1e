#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/Position.h>
#include <Simp1e/sreal.h>
#include <function_pointer.h>

#include <string>

#include "IParallaxEffectLayer.h"

namespace Simp1e {

    struct IParallaxEffectComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("IParallaxEffect")

        enum class Fields : int {
            LayersChanged             = 1 << 0,
            TargetPerspectivePosition = 1 << 1,
        };

        virtual IParallaxEffectLayer* AddLayer(
            const char* layerName, const char* path, sreal speed = 1.0, sreal scale = 1.0f, sreal opacity = 1.0f
        )                                                                         = 0;
        virtual IParallaxEffectLayer* GetLayer(const char* name) const            = 0;
        virtual void                  EnableLayer(const char* layerName)          = 0;
        virtual void                  DisableLayer(const char* layerName)         = 0;
        virtual void                  RemoveLayer(const char* layerName)          = 0;
        virtual bool                  IsLayerEnabled(const char* layerName) const = 0;

        virtual void ForEachLayer(IFunctionPointer<void(IParallaxEffectLayer*)>* callback) const = 0;

        virtual void     SetTargetPerspectivePosition(const Position& position) = 0;
        virtual Position GetTargetPerspectivePosition() const                   = 0;
    };
}