#pragma once

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPen>

#include "IQtGraphicsItem.h"
#include "QtMoveableResizableGraphicsItem.h"
#include "QtUI.h"

namespace Simp1e::UI::Qt {

    class QtGraphicsItem : public IQtGraphicsItem {
        QGraphicsRectItem* _borderRectItem;

    public:
        QtGraphicsItem(QGraphicsItem* parent = nullptr)
            : IQtGraphicsItem(parent), _borderRectItem(nullptr) {
            setFlags(QGraphicsItem::ItemIsMovable);
        }

        QRectF GetBoundingBox() const { return boundingRect(); }

        bool SetBorder(bool enabled, UIColor color = {}, UILineStyle style = UILineStyle::Solid)
            override {
            if (_borderRectItem) {
                delete _borderRectItem;
                _borderRectItem = nullptr;
            }
            if (enabled) {
                _borderRectItem = new QGraphicsRectItem(this);
                _borderRectItem->setPen(QPen(ToQColor(color), 1, ToQtPenStyle(style)));
                UpdateBorder();
            }
            return true;
        }

        void UpdateBorder() {
            if (_borderRectItem != nullptr) {
                _borderRectItem->setRect(boundingRect());
            }
        }

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
            if ((change == QGraphicsItem::ItemPositionHasChanged ||
                 change == QGraphicsItem::ItemScaleHasChanged) &&
                _borderRectItem != nullptr) {
                UpdateBorder();
            }

            return QGraphicsItem::itemChange(change, value);
        }
    };
}
