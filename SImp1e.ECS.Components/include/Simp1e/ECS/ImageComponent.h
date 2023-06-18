#pragma once

#include <Simp1e/ECS/ComponentType.h>
#include <Simp1e/ECS/ComponentTypeMacro.h>

#include <string>

#include "ComponentBase.h"

namespace Simp1e::ECS {
    class ImageComponent : public ComponentBase {
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
