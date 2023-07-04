#pragma once

#include <QGraphicsItem>

namespace Simp1e {

    class QSimp1eGraphicsItem : public QGraphicsItem {
    public:
        QSimp1eGraphicsItem(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {}

        virtual QRectF boundingRect() const override { return QRectF(); }
        virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
            override {}
    };
}
