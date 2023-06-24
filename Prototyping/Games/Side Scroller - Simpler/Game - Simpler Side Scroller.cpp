#include <_Log_.h>
_LogToFile_("SimpleSideScroller.log");
///////////////////////////////////////////

#include <Simp1e/Game.h>
#include <Simp1e/Size.h>

#include <QApplication>
#include <QDebug>
#include <QGraphicsColorizeEffect>
#include <QGraphicsSvgItem>
#include <QScrollBar>
#include <QTimer>

#include "SideScrollerEntities.h"
#include "SideScrollerGraphicsScene.h"
#include "SideScrollerGraphicsView.h"
#include "SideScrollerImages.h"
#include "SideScrollerSystems.h"

using namespace Simp1e;
using namespace Simp1e::ECS;
using namespace Simp1e::SideScroller;

#include <QGraphicsPixmapItem>

class ParallaxLayer : public QGraphicsPixmapItem {
public:
    ParallaxLayer(const QString& path, qreal scrollSpeed) : QGraphicsPixmapItem(QPixmap(path)), speed(scrollSpeed) {}
    ParallaxLayer(const QPixmap& pixmap, qreal scrollSpeed) : QGraphicsPixmapItem(pixmap), speed(scrollSpeed) {}

    qreal speed;
};

#include <QGraphicsView>
#include <QKeyEvent>
#include <QList>
#include <QVector>

class ParallaxScrollingView : public QGraphicsView {
public:
    ParallaxScrollingView(QGraphicsScene* scene) : QGraphicsView(scene) {}

    void addLayer(const QString& path, qreal speed, qreal scale = 1.0) {
        QPixmap pixmap(path);

        if (scale != 1.0) {
            pixmap = pixmap.scaled(pixmap.width() * scale, pixmap.height() * scale);
        }

        int                     requiredInstances = (this->width() / pixmap.width()) + 2;
        QVector<ParallaxLayer*> layerInstances;
        for (int i = 0; i < requiredInstances; ++i) {
            ParallaxLayer* layer = new ParallaxLayer(pixmap, speed);
            layer->setX(i * pixmap.width());
            layerInstances.append(layer);
            scene()->addItem(layer);
        }
        layers.append(layerInstances);
    }

    void scrollRight() {
        for (auto& layerInstances : layers) {
            for (int i = 0; i < layerInstances.size(); ++i) {
                ParallaxLayer* layer = layerInstances[i];
                layer->setX(layer->x() - layer->speed);
                if (layer->x() + layer->pixmap().width() < 0) {
                    // get the last instance's x to ensure the layer is placed immediately after it
                    qreal lastInstanceX = layerInstances[(i + layerInstances.size() - 1) % layerInstances.size()]->x();
                    layer->setX(lastInstanceX + layer->pixmap().width());
                }
            }
        }
    }

    void scrollLeft() {
        for (auto& layerInstances : layers) {
            for (int i = 0; i < layerInstances.size(); ++i) {
                ParallaxLayer* layer = layerInstances[i];
                layer->setX(layer->x() + layer->speed);
                if (layer->x() - layer->pixmap().width() > 0) {
                    // get the next instance's x to ensure the layer is placed immediately before it
                    qreal nextInstanceX = layerInstances[(i + 1) % layerInstances.size()]->x();
                    layer->setX(nextInstanceX - layer->pixmap().width());
                }
            }
        }
    }

protected:
    void keyPressEvent(QKeyEvent* event) override {
        switch (event->key()) {
            case Qt::Key_D:
                scrollRight();
                break;
            case Qt::Key_A:
                scrollLeft();
                break;
            default:
                QGraphicsView::keyPressEvent(event);
        }
    }

private:
    QVector<QVector<ParallaxLayer*>> layers;
};

Game   game;
Size   levelSize{10000, 1000};
QTimer mainLoopTimer;

void GameLoop() { game.Update(); }

// void PlayWithScrollingBackgrounds(QGraphicsScene& scene) {
//     // auto path = "C:/Code/mrowrpurr/StockImages/shutterstock_1382713007.svg";
//     // auto path = "C:/Code/mrowrpurr/StockImages/shutterstock_1575470704.svg";
//     auto path = "C:/Code/mrowrpurr/StockImages/shutterstock_1575470704 - sky removed.svg";
//     auto svg  = new QGraphicsSvgItem(path);

//     // colorize
//     auto effect = new QGraphicsColorizeEffect();
//     effect->setColor(QColor(255, 0, 255));
//     svg->setGraphicsEffect(effect);

//     svg->setPos(0, 0);
//     qDebug() << "Adding svg with bounding rect of" << svg->boundingRect() << "to scene";
//     scene.addItem(svg);
// }

int main(int argc, char* argv[]) {
    /*
        Qt UI
    */

    QApplication app(argc, argv);
    app.setStyle("fusion");

    SideScrollerGraphicsScene scene;  // (game.Events());
    scene.setSceneRect(0, 0, levelSize.width(), levelSize.height());

    ParallaxScrollingView view(&scene);
    view.setWindowTitle("Side Scroller");
    view.move(0, 0);
    view.show();
    view.horizontalScrollBar()->setValue(0);

    // scroll the QGraphicsView all the way to the left

    view.addLayer("C:/Code/mrowrpurr/StockImages/shutterstock_329111897.svg", 0.5);
    view.addLayer("C:/Code/mrowrpurr/StockImages/shutterstock_1575470704 - sky removed.svg", 1.0, 0.4);
    // view.addLayer("C:/Code/mrowrpurr/PublicDomainResources/Openclipart/297667.svg", 1);

    // PlayWithScrollingBackgrounds(scene);
    // SetupSystems();
    // SetupEntities();

    /*
        Start game main() loop and run the application
    */

    // QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    // mainLoopTimer.start(16);
    return app.exec();
}
