#include <QApplication>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
#include <QPen>
#include <QTabletEvent>


class GraphicsView : public QGraphicsView {
public:
    GraphicsView() {
        QGraphicsScene* scene = new QGraphicsScene(this);
        scene->setSceneRect(0, 0, 600, 400);
        setScene(scene);
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
                path.moveTo(event->pos());
                break;
            case QEvent::TabletMove:
                pen.setWidthF(
                    event->pressure() * 10
                );  // Adjust the multiplication factor for pen width as per requirement.
                path.lineTo(event->pos());
                scene()->addPath(path, pen);
                path = QPainterPath();
                path.moveTo(event->pos());
                break;
            case QEvent::TabletRelease:
                scene()->addPath(path, pen);
                path = QPainterPath();
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
