#pragma once

#include <QGraphicsView>

namespace Prototyping {

    class GameGraphicsView : public QGraphicsView {
    public:
        GameGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {}

        void SetScene(QGraphicsScene* scene) { QGraphicsView::setScene(scene); }

        // : _gridMap(map), QGraphicsView(scene, parent) {}

        // protected:
        //     void OnUpKey() {
        //         if (_gridMap) _gridMap->MoveCircleUp();
        //     }
        //     void OnDownKey() {
        //         if (_gridMap) _gridMap->MoveCircleDown();
        //     }
        //     void OnLeftKey() {
        //         if (_gridMap) _gridMap->MoveCircleLeft();
        //     }
        //     void OnRightKey() {
        //         if (_gridMap) _gridMap->MoveCircleRight();
        //     }

        // void keyPressEvent(QKeyEvent* event) override {
        //     switch (event->key()) {
        //         case Qt::Key_Up:
        //             OnUpKey();
        //             break;
        //         case Qt::Key_Down:
        //         case Qt::Key_S:
        //             OnDownKey();
        //             break;
        //         case Qt::Key_Left:
        //         case Qt::Key_A:
        //             OnLeftKey();
        //             break;
        //         case Qt::Key_Right:
        //         case Qt::Key_D:
        //             OnRightKey();
        //             break;
        //         default:
        //             QGraphicsView::keyPressEvent(event);
        //             break;
        //     }
        // }
    };
}
