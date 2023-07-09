#pragma once

#include <function_pointer.h>

#include <QGraphicsItem>

namespace Simp1e {

    class QSimp1eGraphicsItem : public QGraphicsItem {
        QRectF                                                                                        _boundingRect;
        std::unique_ptr<IFunctionPointer<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>> _paintFunction =
            nullptr;

    public:
        QSimp1eGraphicsItem(
            IFunctionPointer<void(QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>* _paintFunctionPointer,
            QGraphicsItem*                                                                parent = nullptr
        )
            : QGraphicsItem(parent), _paintFunction(_paintFunctionPointer) {}

        void SetBoundingRect(const QRectF& boundingRect) { _boundingRect = boundingRect; }

    protected:
        virtual QRectF boundingRect() const override { return _boundingRect; }
        virtual void   paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr)
            override {
            if (_paintFunction) _paintFunction->invoke(painter, option, widget);
        }
    };
}
