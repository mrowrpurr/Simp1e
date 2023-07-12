#pragma once

#include <_Log_.h>
#include <function_pointer.h>

#include <QGraphicsView>
#include <QScrollBar>
#include <QWheelEvent>
#include <memory>
#include <optional>
#include <unordered_set>

namespace Simp1e {

    class QSimp1eGraphicsView : public QGraphicsView {
        QPoint                                                                  _lastPanPoint;
        bool                                                                    _isPanning = false;
        std::unordered_set<std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>> _keyPressListeners;
        std::unordered_set<std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>> _keyReleaseListeners;

    public:
        QSimp1eGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {
            // Enable scrollbars
            // setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            setDragMode(QGraphicsView::NoDrag);
            setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
            setRenderHint(QPainter::Antialiasing);
            setStyleSheet("background: black");  // Set via <FillColor> of a <Canvas>
        }

        void FitSceneToViewHeight() {
            auto sceneRect   = scene()->sceneRect();
            auto scaleFactor = height() / sceneRect.height();
            setTransform(QTransform::fromScale(scaleFactor, scaleFactor));
        }

        void OnKeyPress(IFunctionPointer<void(QKeyEvent*)>* callback) {
            _keyPressListeners.insert(std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>(callback));
        }

        void OnKeyRelease(IFunctionPointer<void(QKeyEvent*)>* callback) {
            _keyReleaseListeners.insert(std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>(callback));
        }

        void OnKeyPress(FunctionPointer<void(QKeyEvent*)> callback) {
            callback.do_not_destroy_function_pointer();
            _keyPressListeners.insert(
                std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>(callback.inner_function_pointer())
            );
        }

        void OnKeyRelease(FunctionPointer<void(QKeyEvent*)> callback) {
            callback.do_not_destroy_function_pointer();
            _keyReleaseListeners.insert(
                std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>(callback.inner_function_pointer())
            );
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

        void keyPressEvent(QKeyEvent* event) override {
            for (auto& listener : _keyPressListeners) listener->invoke(event);
        }

        void keyReleaseEvent(QKeyEvent* event) override {
            for (auto& listener : _keyReleaseListeners) listener->invoke(event);
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
