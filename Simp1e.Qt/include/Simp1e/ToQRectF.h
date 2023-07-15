#pragma once

#include <Simp1e/Rectangle.h>

#include <QRectF>

namespace Simp1e {
    QRectF ToQRectF(const Rectangle& rect) { return QRectF(rect.x(), rect.y(), rect.width(), rect.height()); }
}
