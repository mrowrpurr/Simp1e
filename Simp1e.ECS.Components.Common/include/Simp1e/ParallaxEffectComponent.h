#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "IParallaxEffectComponent.h"
#include "ParallaxEffectLayer.h"

namespace Simp1e {

    class ParallaxEffectComponent : public IParallaxEffectComponent {
        std::unordered_map<std::string, std::unique_ptr<ParallaxEffectLayer>> _layers;
        std::vector<std::string>                                              _layersOrder;
        std::unordered_map<std::string, bool>                                 _layersEnabled;
        Position                                                              _currentTargetPerspectivePosition;

    public:
        ParallaxEffectComponent() = default;
        ParallaxEffectComponent(const std::vector<ParallaxEffectLayer>& layers) {
            for (auto& layer : layers)
                AddLayer(layer.layerName(), layer.imagePath(), layer.speed(), layer.scale(), layer.opacity());
        }
        ParallaxEffectComponent(
            const Position& currentTargetPerspectivePosition, const std::vector<ParallaxEffectLayer>& layers
        )
            : ParallaxEffectComponent(layers) {
            _currentTargetPerspectivePosition = currentTargetPerspectivePosition;
        }

        IParallaxEffectLayer* AddLayer(const char* layerName, const char* path, sreal speed, sreal scale, sreal opacity)
            override {
            auto* layer        = new ParallaxEffectLayer(layerName, path, speed, scale, opacity);
            _layers[layerName] = std::unique_ptr<ParallaxEffectLayer>(layer);
            _layersOrder.push_back(layerName);
            _layersEnabled[layerName] = true;
            SetDirtyFlag(Fields::LayersChanged);
            return layer;
        }

        IParallaxEffectLayer* GetLayer(const char* layerName) const override {
            auto found = _layers.find(layerName);
            if (found != _layers.end()) return found->second.get();
            return nullptr;
        }

        void EnableLayer(const char* layerName) override {
            auto found = _layersEnabled.find(layerName);
            if (found != _layersEnabled.end()) {
                found->second = true;
                SetDirtyFlag(Fields::LayersChanged);
            }
        }

        void DisableLayer(const char* layerName) override {
            auto found = _layersEnabled.find(layerName);
            if (found != _layersEnabled.end()) {
                found->second = false;
                SetDirtyFlag(Fields::LayersChanged);
            }
        }

        void RemoveLayer(const char* layerName) override {
            auto found = _layers.find(layerName);
            if (found != _layers.end()) {
                _layers.erase(found);
                _layersOrder.erase(std::remove(_layersOrder.begin(), _layersOrder.end(), layerName));
                _layersEnabled.erase(layerName);
                SetDirtyFlag(Fields::LayersChanged);
            }
        }

        bool IsLayerEnabled(const char* layerName) const override {
            auto found = _layersEnabled.find(layerName);
            if (found != _layersEnabled.end()) return found->second;
            return false;
        }

        void SetTargetPerspectivePosition(const Position& position) override {
            _currentTargetPerspectivePosition = position;
            SetDirtyFlag(Fields::TargetPerspectivePosition);
        }
        Position GetTargetPerspectivePosition() const override { return _currentTargetPerspectivePosition; }

        void ForEachLayer(IFunctionPointer<void(IParallaxEffectLayer* layer)>* callback) const override {
            for (const auto& layerName : _layersOrder) {
                auto found = _layers.find(layerName);
                if (found != _layers.end()) callback->invoke(found->second.get());
            }
        }
    };
}