#pragma once

#include <Simp1e/ECS/ImageComponent.h>
#include <Simp1e/QT/Conversions/ToQSize.h>
#include <Simp1e/Size.h>

#include <QPixMapCache>
#include <QPixmap>

namespace Simp1e::ECS {
    class QTImageComponent : public ImageComponent {
        QPixmap _pixmap;

    public:
        SIMP1E_ECS_COMPONENT("QTImage")

        QTImageComponent() = default;
        QTImageComponent(const QPixmap& pixmap) : _pixmap(pixmap) {}
        QTImageComponent(const QString& path) : ImageComponent(path.toStdString()) {
            SetPixmap(path);
        }

        virtual QPixmap GetPixmap() const { return _pixmap; }
        virtual QPixmap pixmap() const { return _pixmap; }

        virtual void SetPixmap(const QPixmap& pixmap) {
            _pixmap = pixmap;
            SetDirty();
        }
        virtual void SetPixmap(const QString& path) {
            if (!QPixmapCache::find(path, &_pixmap)) {
                _pixmap = QPixmap(path);
                QPixmapCache::insert(path, _pixmap);
            }
            SetDirty();
        }

        virtual void SetSize(const QSize& size) {
            if (_pixmap.isNull()) return;
            if (_pixmap.width() == size.width() && _pixmap.height() == size.height()) return;
            _pixmap = _pixmap.scaled(size);
            SetDirty();
        }
        virtual void SetSize(const Size& size) { SetSize(ToQSize(size)); }
    };
}
