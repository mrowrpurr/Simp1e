#pragma once

// TODO: use Qt types instead of UI:: stuff
#include <Simp1e/UI/UIColor.h>
#include <Simp1e/UI/UILineStyle.h>

#include <QGraphicsItem>

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
        virtual void UpdateBorder() {}

        virtual bool SetResizingMode(bool enabled, bool maintainAspectRatio = false) {
            return false;
        }
        virtual bool SetMovingMode(bool enabled) { return false; }
        virtual bool SetRotatingMode(bool enabled) { return false; }

    protected:
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {}
        QRectF boundingRect() const override { return {}; }
    };
}
