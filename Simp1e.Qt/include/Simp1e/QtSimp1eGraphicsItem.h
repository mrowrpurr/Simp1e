#pragma once

#include <function_pointer.h>

#include <QGraphicsItem>

namespace Simp1e {

    class QtSimp1eGraphicsItem : public QGraphicsItem {
        QRectF                                                                                        _boundingRect;
        std::unique_ptr<IFunctionPointer<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>> _paintFunction =
            nullptr;

    public:
        QtSimp1eGraphicsItem(
            IFunctionPointer<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>* _paintFunctionPointer,
            QGraphicsItem*                                                                parent = nullptr
        )
            : QGraphicsItem(parent), _paintFunction(_paintFunctionPointer) {}

        void SetBoundingRect(const QRectF& boundingRect) {
            prepareGeometryChange();
            _boundingRect = boundingRect;
            update();
        }

        QRectF GetBoundingRect() const { return _boundingRect; }

    protected:
        virtual QRectF boundingRect() const override { return _boundingRect; }
        virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
            override {
            if (_paintFunction) _paintFunction->invoke(painter, option, widget);
        }
    };
}
