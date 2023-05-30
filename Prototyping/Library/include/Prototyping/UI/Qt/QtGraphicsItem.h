#pragma once

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QPen>

#include "QtMoveableResizableGraphicsItem.h"

namespace Prototyping::UI::Qt {

    class Simp1eQtGraphicsItem : public QGraphicsItem {
    public:
        Simp1eQtGraphicsItem(QGraphicsItem* parent = nullptr)
            : QGraphicsItem(parent), borderItem(nullptr) {
            setFlags(QGraphicsItem::ItemIsMovable);

            // Testing!
            SetBorderVisible(true);
        }

        void SetBorderVisible(bool visible) {
            if (visible && borderItem == nullptr) {
                borderItem = new QGraphicsRectItem(this);
                borderItem->setPen(QPen(::Qt::red, 1, ::Qt::DashLine));
                borderItem->setZValue(zValue() - 1);
                updateBorder();
            } else if (!visible && borderItem != nullptr) {
                delete borderItem;
                borderItem = nullptr;
            }
        }

        QRectF GetBoundingBox() const { return boundingRect(); }

    protected:
        QVariant itemChange(GraphicsItemChange change, const QVariant& value) override {
            if ((change == QGraphicsItem::ItemPositionHasChanged ||
                 change == QGraphicsItem::ItemScaleHasChanged) &&
                borderItem != nullptr) {
                updateBorder();
            }

            return QGraphicsItem::itemChange(change, value);
        }

        void updateBorder() {
            if (borderItem != nullptr) {
                borderItem->setRect(boundingRect());
            }
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            // Empty implementation for MyBaseItem
        }

        QRectF boundingRect() const override {
            // Return default QRectF for MyBaseItem. It should be overridden in derived classes.
            return QRectF();
        }

    private:
        QGraphicsRectItem* borderItem;
    };
}
