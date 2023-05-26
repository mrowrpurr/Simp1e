#pragma once

#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <memory>

namespace Prototyping {

    class GameGraphicsRectItem : public QGraphicsRectItem {
        std::unique_ptr<std::function<void()>> _onLeftClickHandler;
        std::unique_ptr<std::function<void()>> _onRightClickHandler;

    public:
        GameGraphicsRectItem(QRectF rect, QGraphicsItem* parent = nullptr)
            : QGraphicsRectItem(rect, parent) {}

        void OnLeftClick(std::function<void()> handler) {
            _onLeftClickHandler = std::make_unique<std::function<void()>>(handler);
        }
        void OnRightClick(std::function<void()> handler) {
            _onRightClickHandler = std::make_unique<std::function<void()>>(handler);
        }

    protected:
        void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                if (_onLeftClickHandler) (*_onLeftClickHandler)();
            } else if (event->button() == Qt::RightButton) {
                if (_onRightClickHandler) (*_onRightClickHandler)();
            }
            QGraphicsRectItem::mousePressEvent(event);
        }
    };
}