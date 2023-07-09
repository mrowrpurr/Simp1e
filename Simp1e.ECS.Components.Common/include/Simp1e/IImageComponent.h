#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/DirtyTrackingComponentBase.h>
#include <Simp1e/ImageRenderType.h>
#include <void_pointer.h>

namespace Simp1e {

    struct IImageComponent : public DirtyTrackingComponentBase {
        DEFINE_COMPONENT_TYPE("Image")

        enum class Fields : int {
            ImagePath = 1 << 0,
            ImageData = 1 << 1,
        };

        virtual ImageRenderType GetImageRenderType() const = 0;

        virtual const char* GetImagePath() const           = 0;
        virtual void        SetImagePath(const char* path) = 0;

        virtual void* GetImageData() const             = 0;
        virtual void  SetImageData(IVoidPointer* data) = 0;
    };
}