#pragma once

#include <QGraphicsObject>
#include <QPainter>

namespace Prototyping {

    class AnimatedEllipse : public QGraphicsObject {
    public:
        AnimatedEllipse(qreal x, qreal y, qreal w, qreal h) : rect(x, y, w, h) { setPos(x, y); }

        QRectF boundingRect() const override { return rect; }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            painter->setBrush(Qt::blue);
            painter->drawEllipse(rect);
        }

    private:
        QRectF rect;
    };
}
