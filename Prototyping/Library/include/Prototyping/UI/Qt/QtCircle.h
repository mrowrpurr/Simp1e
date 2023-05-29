#pragma once

#include <QGraphicsObject>
#include <QPainter>

#include "../UIColor.h"

namespace Prototyping::UI::Qt {
    class QtCircle : public QGraphicsObject {
        QRectF  _rect;
        UIColor _color;

    public:
        QtCircle(const UIColor& color, uint32_t diameter)
            : _rect(0, 0, diameter, diameter), _color(color) {}

        QRectF boundingRect() const override { return _rect; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            painter->setBrush(QBrush(QColor(_color.red, _color.green, _color.blue, _color.alpha)));
            painter->drawEllipse(_rect);
        }
    };
}
