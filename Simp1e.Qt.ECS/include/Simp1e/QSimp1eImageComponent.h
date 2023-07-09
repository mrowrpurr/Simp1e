#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/QSimp1eImage.h>

namespace Simp1e {

    class QSimp1eImageComponent {
        std::unique_ptr<QSimp1eImage> _image;

    public:
        DEFINE_COMPONENT_TYPE("QSimp1eImage")

        QSimp1eImageComponent(const char* imagePath) : _image(std::make_unique<QSimp1eImage>(imagePath)) {}

        QSimp1eImage* GetQSimp1eImage() const { return _image.get(); }
    };
}
