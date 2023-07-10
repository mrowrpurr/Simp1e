#pragma once

#include <string>
#include <vector>

#include "IParallaxEffectComponent.h"

namespace Simp1e {

    class ParallaxEffectComponent : public IParallaxEffectComponent {
    public:
        struct ParallaxEffectLayer {
            std::string path;
            sreal       speed;
            sreal       scale;
            bool        enabled;
        };

    private:
        std::vector<ParallaxEffectLayer> layers;
        Position                         _currentTargetPerspectivePosition;

    public:
        ParallaxEffectComponent() = default;
        ParallaxEffectComponent(const std::vector<ParallaxEffectLayer>& layers) : layers(layers) {}
        ParallaxEffectComponent(
            const Position& currentTargetPerspectivePosition, const std::vector<ParallaxEffectLayer>& layers
        )
            : _currentTargetPerspectivePosition(currentTargetPerspectivePosition), layers(layers) {}

        int AddLayer(const char* path, sreal speed, sreal scale) override {
            layers.push_back({path, speed, scale, true});
            SetDirtyFlag(Fields::LayersChanged);
            return layers.size() - 1;
        }

        void EnableLayer(int index) override {
            if (index < 0 || index >= layers.size()) return;
            layers[index].enabled = true;
            SetDirtyFlag(Fields::LayersChanged);
        }

        void DisableLayer(int index) override {
            if (index < 0 || index >= layers.size()) return;
            layers[index].enabled = false;
            SetDirtyFlag(Fields::LayersChanged);
        }

        void RemoveLayer(int index) override {
            if (index < 0 || index >= layers.size()) return;
            layers.erase(layers.begin() + index);
            SetDirtyFlag(Fields::LayersChanged);
        }

        bool IsLayerEnabled(int index) const override {
            if (index < 0 || index >= layers.size()) return false;
            return layers[index].enabled;
        }

        void SetTargetPerspectivePosition(const Position& position) override {
            _currentTargetPerspectivePosition = position;
            SetDirtyFlag(Fields::TargetPerspectivePosition);
        }
        Position GetTargetPerspectivePosition() const override { return _currentTargetPerspectivePosition; }
    };
}