#include <QApplication>
#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPainterPath>
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
        switch (event->type()) {
            case QEvent::TabletPress:
                path.moveTo(event->pos());
                break;
            case QEvent::TabletMove:
                path.lineTo(event->pos());
                scene()->addPath(path);
                path = QPainterPath();
                path.moveTo(event->pos());
                break;
            case QEvent::TabletRelease:
                scene()->addPath(path);
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
