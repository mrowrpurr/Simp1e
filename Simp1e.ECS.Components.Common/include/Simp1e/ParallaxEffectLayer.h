#pragma once

#include <string>

#include "IParallaxEffectLayer.h"

namespace Simp1e {

    class ParallaxEffectLayer : public IParallaxEffectLayer {
        std::string _layerName;
        std::string _imagePath;
        sreal       _speed;
        sreal       _scale;
        sreal       _opacity;
        bool        _enabled;

    public:
        ParallaxEffectLayer() = default;
        ParallaxEffectLayer(
            const char* layerName, const char* imagePath, sreal speed, sreal scale, sreal opacity, bool enabled = true
        )
            : _layerName(layerName),
              _imagePath(imagePath),
              _speed(speed),
              _scale(scale),
              _opacity(opacity),
              _enabled(enabled) {}

        const char* layerName() const override { return _layerName.c_str(); }
        const char* imagePath() const override { return _imagePath.c_str(); }
        sreal       speed() const override { return _speed; }
        sreal       scale() const override { return _scale; }
        sreal       opacity() const override { return _opacity; }
        bool        enabled() const override { return _enabled; }
    };
}