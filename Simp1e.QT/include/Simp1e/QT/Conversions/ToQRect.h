#pragma once

#include <Simp1e/Rectangle.h>

#include <QRect>

namespace Simp1e {
    QRect ToQRect(const Rectangle& rect) {
        return QRect(rect.x(), rect.y(), rect.width(), rect.height());
    }
}
