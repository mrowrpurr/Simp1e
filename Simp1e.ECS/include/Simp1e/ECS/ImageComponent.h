#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include <string>

#include "DirtyTrackingComponent.h"

namespace Simp1e::ECS {
    class ImageComponent : public DirtyTrackingComponent {
        std::string _imagePath;

    public:
        SIMP1E_ECS_COMPONENT("Image")

        ImageComponent() = default;
        ImageComponent(const std::string& imagePath) : _imagePath(imagePath) {}

        virtual std::string GetImagePath() const { return _imagePath; }

        virtual void SetImagePath(const std::string& imagePath) {
            _imagePath = imagePath;
            SetDirty();
        }
    };
}
