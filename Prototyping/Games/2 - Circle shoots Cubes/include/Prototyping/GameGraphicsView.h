#pragma once

#include <QGraphicsView>
#include <QKeyEvent>
#include <functional>
#include <memory>

namespace Prototyping {

    class GameGraphicsView : public QGraphicsView {
        std::unique_ptr<std::function<void()>> _upKeyHandler;
        std::unique_ptr<std::function<void()>> _downKeyHandler;
        std::unique_ptr<std::function<void()>> _leftKeyHandler;
        std::unique_ptr<std::function<void()>> _rightKeyHandler;

    public:
        GameGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

        void SetScene(QGraphicsScene* scene) { QGraphicsView::setScene(scene); }

        void OnUpKey(std::function<void()> handler) {
            _upKeyHandler = std::make_unique<std::function<void()>>(handler);
        }
        void OnDownKey(std::function<void()> handler) {
            _downKeyHandler = std::make_unique<std::function<void()>>(handler);
        }
        void OnLeftKey(std::function<void()> handler) {
            _leftKeyHandler = std::make_unique<std::function<void()>>(handler);
        }
        void OnRightKey(std::function<void()> handler) {
            _rightKeyHandler = std::make_unique<std::function<void()>>(handler);
        }

    protected:
        void keyPressEvent(QKeyEvent* event) override {
            switch (event->key()) {
                case Qt::Key_Up:
                    if (_upKeyHandler) (*_upKeyHandler)();
                    break;
                case Qt::Key_Down:
                case Qt::Key_S:
                    if (_downKeyHandler) (*_downKeyHandler)();
                    break;
                case Qt::Key_Left:
                case Qt::Key_A:
                    if (_leftKeyHandler) (*_leftKeyHandler)();
                    break;
                case Qt::Key_Right:
                case Qt::Key_D:
                    if (_rightKeyHandler) (*_rightKeyHandler)();
                    break;
                default:
                    QGraphicsView::keyPressEvent(event);
                    break;
            }
        }
    };
}
