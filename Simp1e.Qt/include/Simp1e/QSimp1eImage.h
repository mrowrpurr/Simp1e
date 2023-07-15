#pragma once

#include <Simp1e/ImageRenderType.h>
#include <Simp1e/Size.h>
#include <_Log_.h>

#include <QGraphicsSvgItem>
#include <QImage>
#include <QPainter>
#include <QPixmap>
#include <memory>
#include <string>

namespace Simp1e {

    class QSimp1eImage {
        std::string              _imagePath;
        std::unique_ptr<QPixmap> _originalPixmap = nullptr;
        std::unique_ptr<QPixmap> _scaledPixmap   = nullptr;  // rename to something else, cause we scale and rotate
        std::unique_ptr<QGraphicsSvgItem> _svg;

    public:
        QSimp1eImage() = default;
        QSimp1eImage(const char* imagePath) : _imagePath(imagePath) {
            _Log_("QSimp1eImage: {}", _imagePath);
            if (_imagePath.find(".svg") != std::string::npos)
                _svg = std::make_unique<QGraphicsSvgItem>(_imagePath.c_str());
            else _originalPixmap = std::make_unique<QPixmap>(_imagePath.c_str());
        }

        QPixmap*          GetPixmap() const { return _scaledPixmap ? _scaledPixmap.get() : _originalPixmap.get(); }
        QGraphicsSvgItem* GetSvg() const { return _svg.get(); }

        ImageRenderType GetImageRenderType() const { return _svg ? ImageRenderType::Vector : ImageRenderType::Raster; }

        void SetSize(Size size) {
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
            if (!size.IsNull() && !_originalPixmap->isNull()) {
                if (size.width() == 0)
                    size.SetWidth(_originalPixmap->width() * (size.height() / _originalPixmap->height()));
                else if (size.height() == 0)
                    size.SetHeight(
                        _originalPixmap->height() * static_cast<int>(size.width() / _originalPixmap->width())
                    );
            }
            _Log_("Resizing QPixmap to {}x{}", size.width(), size.height());
            _scaledPixmap = std::make_unique<QPixmap>(_originalPixmap->scaled(size.width(), size.height()));
        }

        void Rotate(sreal rotation) {
            if (GetImageRenderType() == ImageRenderType::Vector) {
                _Log_("SVG (rotate) not supported yet");
                return;
            }
            auto image        = GetPixmap()->toImage();
            auto rotatedImage = QImage(image.size(), QImage::Format_ARGB32_Premultiplied);
            rotatedImage.fill(Qt::transparent);

            auto painter = QPainter(&rotatedImage);
            painter.setRenderHint(QPainter::Antialiasing);
            painter.setRenderHint(QPainter::SmoothPixmapTransform);
            painter.translate(rotatedImage.size().width() / 2.0, rotatedImage.size().height() / 2.0);
            painter.rotate(rotation);
            painter.translate(-image.size().width() / 2.0, -image.size().height() / 2.0);
            painter.drawImage(0, 0, image);
            painter.end();

            _scaledPixmap = std::make_unique<QPixmap>(QPixmap::fromImage(rotatedImage));
        }
    };
}
