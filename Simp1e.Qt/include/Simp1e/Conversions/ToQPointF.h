#pragma once

#include <Simp1e/Point.h>

#include <QPointF>

namespace Simp1e {
    QPointF ToQPointF(const Point& point) { return QPointF(point.x(), point.y()); }
}
