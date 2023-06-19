#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include <string>

#include "DirtyTrackingComponent.h"

namespace Simp1e::ECS {
    class ImageComponent : public DirtyTrackingComponent {
        std::string _imageFilename;

    public:
        SIMP1E_ECS_COMPONENT("Image")

        ImageComponent() = default;
        ImageComponent(const std::string& imageFilename) : _imageFilename(imageFilename) {}

        virtual std::string GetImageFilename() const { return _imageFilename; }

        virtual void SetImageFilename(const std::string& imageFilename) {
            _imageFilename = imageFilename;
            SetDirty();
        }
    };
}
