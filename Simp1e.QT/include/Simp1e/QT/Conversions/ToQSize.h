#pragma once

#include <Simp1e/Size.h>

#include <QSize>

namespace Simp1e {
    QSize ToQSize(const Size& size) { return QSize(size.width(), size.height()); }
}
