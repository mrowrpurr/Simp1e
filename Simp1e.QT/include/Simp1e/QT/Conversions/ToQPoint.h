#pragma once

#include <Simp1e/Point.h>

#include <QPoint>

namespace Simp1e {
    QPoint ToQPoint(const Point& point) { return QPoint(point.x(), point.y()); }
}
