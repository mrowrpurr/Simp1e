#pragma once

#include <_Log_.h>

#include <QGraphicsItem>
#include <QPainter>

namespace Simp1e {

    class QSimp1eGraphicsItem : public QGraphicsItem {
    public:
        QSimp1eGraphicsItem(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {}

        virtual QRectF boundingRect() const override { return {0, 0, 100, 100}; }
        virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
            override {
            _Log_("paint() graphics item");

            // paint a red rectangle
            painter->setBrush(Qt::red);
            painter->drawRect(0, 0, 100, 100);
        }
    };
}
