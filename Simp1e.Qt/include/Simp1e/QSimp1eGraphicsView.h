#pragma once

#include <_Log_.h>

#include <QGraphicsView>
#include <QScrollBar>
#include <QWheelEvent>
#include <optional>

namespace Simp1e {

    class QSimp1eGraphicsView : public QGraphicsView {
        QPoint _lastPanPoint;
        bool   _isPanning = false;

    public:
        QSimp1eGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {
            // Enable scrollbars
            // setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            setDragMode(QGraphicsView::NoDrag);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setRenderHint(QPainter::Antialiasing);
            setStyleSheet("background: magenta");
        }

        void FitSceneToViewHeight() {
            auto sceneRect   = scene()->sceneRect();
            auto scaleFactor = height() / sceneRect.height();
            setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
        }

    protected:
        void resizeEvent(QResizeEvent* event) override {
            // _Log_("RESIZE");
            // FitSceneToViewHeight();
            // auto topLeft = mapToScene(0, 0);
            // _eventManager.Emit<ResizeEvent>({
            //     {static_cast<sreal>(topLeft.x()),           static_cast<sreal>(topLeft.y())           },
            //     {static_cast<sreal>(event->size().width()), static_cast<sreal>(event->size().height())}
            // });
            // for (auto item : scene()->items()) item->update();
        }

        void wheelEvent(QWheelEvent* event) override {
            if (event->angleDelta().y() > 0) {  // If scrolling up
                scale(1.1, 1.1);
            } else {  // If scrolling down
                scale(0.9, 0.9);
            }
        }

        void mousePressEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                _lastPanPoint = event->pos();
                _isPanning    = true;
            }
        }

        void mouseMoveEvent(QMouseEvent* event) override {
            if (_isPanning) {
                float panSpeed = 1.0f;  // Adjust this value for faster or slower panning
                auto  offset   = ((_lastPanPoint - event->pos()) * panSpeed);
                horizontalScrollBar()->setValue(horizontalScrollBar()->value() + offset.x());
                verticalScrollBar()->setValue(verticalScrollBar()->value() + offset.y());
                _lastPanPoint = event->pos();
            }
        }

        void mouseReleaseEvent(QMouseEvent* event) override {
            if (event->button() == Qt::LeftButton) {
                _isPanning = false;
            }
        }
    };
}
