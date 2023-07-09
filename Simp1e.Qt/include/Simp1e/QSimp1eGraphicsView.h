#pragma once

#include <_Log_.h>

#include <QGraphicsView>
#include <optional>

namespace Simp1e {

    class QSimp1eGraphicsView : public QGraphicsView {
    public:
        QSimp1eGraphicsView(QWidget* parent = nullptr) : QGraphicsView(parent) {
            // Enable scrollbars
            // setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
            // setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
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
    };
}
