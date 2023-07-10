#pragma once

#include <Simp1e/sreal.h>

namespace Simp1e {

    struct IParallaxEffectLayer {
        virtual const char* layerName() const = 0;
        virtual const char* imagePath() const = 0;
        virtual sreal       speed() const     = 0;
        virtual sreal       scale() const     = 0;
        virtual sreal       opacity() const   = 0;
        virtual bool        enabled() const   = 0;
    };
}