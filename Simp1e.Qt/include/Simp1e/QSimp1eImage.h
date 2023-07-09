#pragma once

#include <Simp1e/ImageRenderType.h>
#include <Simp1e/Size.h>
#include <_Log_.h>

#include <QGraphicsSvgItem>
#include <QPixmap>
#include <memory>
#include <string>

namespace Simp1e {

    class QSimp1eImage {
        std::string                       _imagePath;
        std::unique_ptr<QPixmap>          _originalPixmap = nullptr;
        std::unique_ptr<QPixmap>          _scaledPixmap   = nullptr;
        std::unique_ptr<QGraphicsSvgItem> _svg;

    public:
        QSimp1eImage() = default;
        QSimp1eImage(const char* imagePath) : _imagePath(imagePath) {
            _Log_("QSimp1eImage: {}", _imagePath);
            if (_imagePath.find(".svg") != std::string::npos)
                _svg = std::make_unique<QGraphicsSvgItem>(_imagePath.c_str());
            else _originalPixmap = std::make_unique<QPixmap>(_imagePath.c_str());

            if (_originalPixmap && _originalPixmap->isNull()) _Log_("Original pixmap was CREATED but is NULL");
            else if (_originalPixmap) _Log_("Original pixmap was CREATED and is NOT NULL");
        }

        QPixmap*          GetPixmap() const { return _scaledPixmap ? _scaledPixmap.get() : _originalPixmap.get(); }
        QGraphicsSvgItem* GetSvg() const { return _svg.get(); }

        ImageRenderType GetImageRenderType() const { return _svg ? ImageRenderType::Vector : ImageRenderType::Raster; }

        void SetSize(const Size& size) {
            if (GetImageRenderType() == ImageRenderType::Vector) {
                _Log_("SVG (resize) not supported yet");
                return;
            }
            if (!_originalPixmap) {
                _Log_("No original pixmap");
                return;
            }
            if (_originalPixmap->isNull()) {
                _Log_("Original pixmap is null");
                return;
            }
            if (_scaledPixmap && _scaledPixmap->width() == size.width() && _scaledPixmap->height() == size.height())
                return;
            if (_originalPixmap && _originalPixmap->width() == size.width() &&
                _originalPixmap->height() == size.height())
                return;
            _Log_("Resizing QPixmap to {}x{}", size.width(), size.height());
            _scaledPixmap = std::make_unique<QPixmap>(_originalPixmap->scaled(size.width(), size.height()));
        }
    };
}
