#pragma once

#include <Simp1e/UI/UIColor.h>
#include <Simp1e/UI/UILineStyle.h>

#include <QGraphicsItem>

#include "QtMoveableResizableGraphicsItem.h"

namespace Simp1e::UI::Qt {

    struct IQtGraphicsItem : public QGraphicsItem {
        IQtGraphicsItem(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {}
        virtual ~IQtGraphicsItem() = default;

        virtual QRectF GetBoundingBox() { return boundingRect(); }

        virtual bool SetBorder(
            bool enabled, UIColor color = {}, UILineStyle style = UILineStyle::Solid
        ) {
            return false;
        }

    protected:
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {}
        QRectF boundingRect() const override { return {}; }
    };
}
