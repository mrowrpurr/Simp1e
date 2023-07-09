#pragma once

#include <string>

#include "IImageComponent.h"

namespace Simp1e {

    class ImageComponent : public IImageComponent {
        std::string     _imagePath;
        IVoidPointer*   _imageData;
        ImageRenderType _imageRenderType;

    public:
        ImageComponent() = default;
        ImageComponent(ImageRenderType renderType, const char* imagePath)
            : _imageRenderType(renderType), _imagePath(imagePath) {}
        ImageComponent(const char* imagePath) : _imagePath(imagePath) {
            if (_imagePath.find(".svg") != std::string::npos) _imageRenderType = ImageRenderType::Vector;
            else _imageRenderType = ImageRenderType::Raster;
        }
        ImageComponent(ImageRenderType renderType, IVoidPointer* imageData)
            : _imageRenderType(renderType), _imageData(imageData) {}

        ImageRenderType GetImageRenderType() const override { return _imageRenderType; }

        const char* GetImagePath() const override { return _imagePath.c_str(); }
        void        SetImagePath(const char* path) override {
            _imagePath = path;
            SetDirtyFlag(Fields::ImagePath);
        }

        void* GetImageData() const override { return _imageData; }
        void  SetImageData(IVoidPointer* data) override {
            _imageData = data;
            SetDirtyFlag(Fields::ImageData);
        }
    };
}