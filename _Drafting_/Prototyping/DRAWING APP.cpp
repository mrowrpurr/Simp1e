#include <QApplication>
#include <QDebug>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QPen>
#include <QTabletEvent>

class GraphicsView : public QGraphicsView {
    QPen          _pen{Qt::black, 10};
    QPainterPath* _mousePath;

public:
    GraphicsView() {
        QGraphicsScene* scene          = new QGraphicsScene(this);
        auto            screenGeometry = QGuiApplication::primaryScreen()->geometry();
        setScene(scene);
        scene->addRect(
            0, 0, screenGeometry.width() + 500, screenGeometry.height() + 500, QPen(Qt::white), QBrush(Qt::white)
        );
    }

    void mousePressEvent(QMouseEvent* event) override {
        _mousePath = new QPainterPath();
        _mousePath->moveTo(mapToScene(event->position().toPoint()));
        event->accept();
    }

    void mouseMoveEvent(QMouseEvent* event) override {
        _mousePath->lineTo(mapToScene(event->position().toPoint()));
        scene()->addPath(*_mousePath, _pen);
        _mousePath = new QPainterPath();
        _mousePath->moveTo(mapToScene(event->position().toPoint()));
        event->accept();
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        scene()->addPath(*_mousePath, _pen);
        event->accept();
    }

    void tabletEvent(QTabletEvent* event) override {
        static QPainterPath path;
        static QPen         pen;
        pen.setColor(Qt::black);
        switch (event->type()) {
            case QEvent::TabletPress:
                pen.setWidthF(
                    event->pressure() * 10
                );  // Adjust the multiplication factor for pen width as per requirement.
                path.moveTo(mapToScene(event->position().toPoint()));
                break;
            case QEvent::TabletMove:
                pen.setWidthF(
                    event->pressure() * 10
                );  // Adjust the multiplication factor for pen width as per requirement.
                path.lineTo(mapToScene(event->position().toPoint()));
                scene()->addPath(path, pen);
                path = QPainterPath();
                path.moveTo(mapToScene(event->position().toPoint()));
                break;
            case QEvent::TabletRelease:
                scene()->addPath(path, pen);
                path = QPainterPath();
                break;
            default:
                break;
        }
        event->accept();
    }
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);

    GraphicsView view;
    view.show();

    return app.exec();
}
