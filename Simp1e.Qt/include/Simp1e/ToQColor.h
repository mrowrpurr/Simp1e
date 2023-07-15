#pragma once

#include <Simp1e/Color.h>

#include <QColor>

namespace Simp1e {
    QColor ToQColor(const Color& color) { return QColor(color.red(), color.green(), color.blue(), color.alpha()); }
}
