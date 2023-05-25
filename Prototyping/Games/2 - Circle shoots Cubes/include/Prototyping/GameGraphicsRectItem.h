#pragma once

#include <QGraphicsRectItem>

namespace Prototyping {

    class GameGraphicsRectItem : public QGraphicsRectItem {
        std::function<void()> _onClickHandler      = []() {};
        std::function<void()> _onRightClickHandler = []() {};

    public:
        GameGraphicsRectItem(QRectF rect, QGraphicsItem* parent = nullptr)
            : QGraphicsRectItem(rect, parent) {}

        void OnClick(std::function<void()> handler) { _onClickHandler = handler; }
        void OnRightClick(std::function<void()> handler) { _onRightClickHandler = handler; }

        // protected:
        //     void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        //         if (event->button() == Qt::LeftButton) {
        //             _onClickHandler();
        //         } else if (event->button() == Qt::RightButton) {
        //             _onRightClickHandler();
        //         }
        //         QGraphicsRectItem::mousePressEvent(event);
        //     }
    };
}