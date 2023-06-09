#pragma once

#include <QBitmap>
#include <QFile>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include <QTransform>
#include <memory>

#include "QtGraphicsItem.h"

namespace Simp1e::UI::Qt {
    class QtImage : public QtGraphicsItem {
        QPixmap                  _originalImage;
        QString                  _imagePath;
        std::unique_ptr<QPixmap> _transformedImage;

        QPolygonF _polygon;
        QRectF    _ellipse;

        bool _resize       = false;
        bool _imageChanged = false;
        int  _width        = 0;
        int  _height       = 0;
        int  _rotation     = 0;

    public:
        QtImage(const QString& imagePath = "", QGraphicsItem* parent = nullptr)
            : QtGraphicsItem(parent) {
            if (!imagePath.isEmpty()) SetImage(imagePath);
        }

        void SetResize(bool resize) {
            if (_resize != resize) {
                _resize       = resize;
                _imageChanged = true;
            }
        }

        void SetSize(int width, int height) {
            if (width > 0 && height > 0 && (_width != width || _height != height)) {
                _width        = width;
                _height       = height;
                _imageChanged = true;
            }
        }

        void SetPolygon(const QPolygonF& polygon) {
            if (_polygon != polygon) {
                _polygon      = polygon;
                _width        = polygon.boundingRect().width();
                _height       = polygon.boundingRect().height();
                _imageChanged = true;
            }
        }

        void SetEllipse(int radiusX, int radiusY) {
            QRectF newEllipse(0, 0, radiusX, radiusY);
            if (_ellipse != newEllipse) {
                _ellipse      = newEllipse;
                _width        = radiusX;
                _height       = radiusY;
                _imageChanged = true;
            }
        }

        void SetRotate(int degrees) {
            if (_rotation != degrees) {
                _rotation     = degrees;
                _imageChanged = true;
            }
        }

        void UpdateImage() {
            if (_imageChanged && _transformedImage) {
                auto startingBoundingBox = _transformedImage->rect();

                // if (_resize) {
                _transformedImage = std::make_unique<QPixmap>(
                    _originalImage.scaled(_width, _height)  // , ::Qt::KeepAspectRatio)
                );
                // }

                QTransform transform;
                transform.rotate(_rotation);
                _transformedImage =
                    std::make_unique<QPixmap>(_transformedImage->transformed(transform));

                // Create a mask for the original image
                QImage mask(_transformedImage->size(), QImage::Format_Alpha8);
                mask.fill(::Qt::transparent);

                QPainter painter(&mask);
                painter.setBrush(QBrush(::Qt::white, ::Qt::SolidPattern));

                // If we have a polygon, draw it on the mask
                if (!_polygon.isEmpty()) {
                    painter.drawPolygon(_polygon);
                }

                // If we have an ellipse, draw it on the mask
                else if (!_ellipse.isEmpty()) {
                    painter.drawEllipse(_ellipse);
                }

                painter.end();

                // Use the mask to remove unwanted areas from the original image
                QBitmap bitmapMask = QBitmap::fromImage(mask);
                _transformedImage->setMask(bitmapMask);

                _imageChanged = false;

                // Change the position so that the image stays in the same place
                auto newBoundingBox = _transformedImage->rect();
                auto heightChange   = newBoundingBox.height() - startingBoundingBox.height();
                auto widthChange    = newBoundingBox.width() - startingBoundingBox.width();

                // setPos so that the bottom left of the image stays in the same place
                setPos(pos().x() - widthChange / 2, pos().y() - heightChange);

                UpdateBorder();
            }
        }

        int GetWidth() {
            if (!_transformedImage) return 0;
            UpdateImage();
            return _transformedImage->width();
        }

        int GetHeight() {
            if (!_transformedImage) return 0;
            UpdateImage();
            return _transformedImage->height();
        }

        std::unique_ptr<QPixmap>& GetImage() {
            UpdateImage();
            return _transformedImage;
        }

        void SetImage(const QString& imagePath) {
            if (imagePath.isEmpty()) return;
            if (!QFile::exists(imagePath)) return;
            _imagePath        = imagePath;  // just for logging
            _originalImage    = QPixmap{imagePath};
            _transformedImage = std::make_unique<QPixmap>(_originalImage);
            _width            = _transformedImage->width();
            _height           = _transformedImage->height();
        }

        void SetImage(QPixmap image) {
            _originalImage    = image;
            _transformedImage = std::make_unique<QPixmap>(image);
        }

        void SetBoundingBox(const QRectF& boundingBox) override {
            QtGraphicsItem::SetBoundingBox(boundingBox);
            SetSize(boundingBox.width(), boundingBox.height());
            UpdateImage();
        }

        QRectF boundingRect() const override {
            return _transformedImage ? _transformedImage->rect() : QRectF();
        }

    protected:
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            if (!_transformedImage) return;
            UpdateImage();
            painter->drawPixmap(_transformedImage->rect(), *_transformedImage);
        }
    };
}
