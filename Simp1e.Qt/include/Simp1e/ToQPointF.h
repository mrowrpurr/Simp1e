#pragma once

#include <Simp1e/Point.h>
#include <Simp1e/Position.h>
#include <Simp1e/Vec2.h>
#include <Simp1e/Vec3.h>

#include <QPointF>

namespace Simp1e {
    QPointF ToQPointF(const Point& point) { return QPointF(point.x(), point.y()); }

    template <typename T>
    QPointF ToQPointF(const Vec2<T>& vec) {
        return QPointF(static_cast<qreal>(vec.one()), static_cast<qreal>(vec.two()));
    }

    QPointF ToQPointF(const Position& pos) { return QPointF(pos.x(), pos.y()); }

    template <typename T>
    QPointF ToQPointF(const Vec3<T>& vec) {
        return QPointF(static_cast<qreal>(vec.one()), static_cast<qreal>(vec.two()));
    }
}
