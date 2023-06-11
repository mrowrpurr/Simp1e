#pragma once

#include <QGraphicsItem>

namespace SideScroller {

    struct IUILevel;

    struct IUILevelItem : public QGraphicsItem {
        IUILevelItem(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {}
        virtual ~IUILevelItem()      = default;
        virtual IUILevel* GetLevel() = 0;

    protected:
        QRectF boundingRect() const override { return QRectF(); }
        void   paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}
    };
}
