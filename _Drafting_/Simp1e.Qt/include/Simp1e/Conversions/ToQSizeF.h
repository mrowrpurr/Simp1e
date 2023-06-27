#pragma once

#include <Simp1e/Size.h>

#include <QSizeF>

namespace Simp1e {
    QSizeF ToQSizeF(const Size& size) { return QSizeF(size.width(), size.height()); }
}
