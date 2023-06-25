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

    void addLayer(const QString& path, qreal speed, qreal scale = 1.0, QColor color = {0, 0, 0}) {
        QPixmap pixmap(path);
        if (scale != 1.0) pixmap = pixmap.scaled(pixmap.width() * scale, pixmap.height() * scale);

        int                     requiredInstances = (this->width() / pixmap.width()) + 2;
        QVector<ParallaxLayer*> layerInstances;
        for (int i = 0; i < requiredInstances; ++i) {
            ParallaxLayer* layer = new ParallaxLayer(pixmap, speed);
            layer->setX(i * pixmap.width());
            layerInstances.append(layer);
            if (color != QColor(0, 0, 0)) {
                // HEAP memory leak - TODO fix later :)
                QGraphicsColorizeEffect* effect = new QGraphicsColorizeEffect;
                effect->setColor(color);
                effect->setStrength(0.6);
                layer->setGraphicsEffect(effect);
            }
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
Size   levelSize{10000, 1200};
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
    view.addLayer("C:/Code/mrowrpurr/StockImages/shutterstock_1174030309.svg", 2.0, 0.85);

    // view.addLayer("C:/Code/mrowrpurr/StockImages/shutterstock_1008024175 - with alpha.svg", 3.0, 2.1);
    // view.addLayer("C:/Code/mrowrpurr/StockImages/shutterstock_1008024175-with-alpha.png", 3.0, 0.7, QColor{59, 29,
    // 0});

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

/////////////////////////////////

#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QTabWidget>

int main2(int argc, char** argv) {
    QApplication app(argc, argv);

    QMainWindow mainWindow;
    mainWindow.setWindowTitle("This is the main window title");

    QWidget* widgetA = new QWidget;
    QLabel*  labelA  = new QLabel("Widget A", widgetA);

    mainWindow.setCentralWidget(widgetA);

    QDockWidget* topDock = new QDockWidget("Widget B", &mainWindow);
    QLabel*      labelB  = new QLabel("Widget B", topDock);
    topDock->setWidget(labelB);
    mainWindow.addDockWidget(Qt::TopDockWidgetArea, topDock);

    QDockWidget* bottomDock = new QDockWidget("Widget C", &mainWindow);
    QLabel*      labelC     = new QLabel("Widget C", bottomDock);
    bottomDock->setWidget(labelC);
    mainWindow.addDockWidget(Qt::BottomDockWidgetArea, bottomDock);

    QDockWidget* tabbedDock  = new QDockWidget("Already tabbed", &mainWindow);
    QLabel*      labelTabbed = new QLabel("Already tabbed", tabbedDock);
    tabbedDock->setWidget(labelTabbed);
    mainWindow.tabifyDockWidget(bottomDock, tabbedDock);

    QDockWidget* leftDock = new QDockWidget("Widget D", &mainWindow);
    QLabel*      labelD   = new QLabel("Widget D", leftDock);
    leftDock->setWidget(labelD);
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, leftDock);

    QDockWidget* rightDock = new QDockWidget("Widget E", &mainWindow);
    QLabel*      labelE    = new QLabel("Widget E", rightDock);
    rightDock->setWidget(labelE);
    mainWindow.addDockWidget(Qt::RightDockWidgetArea, rightDock);

    QDockWidget* tabDock   = new QDockWidget("Widget F", &mainWindow);
    QTabWidget*  tabWidget = new QTabWidget(tabDock);
    QLabel*      labelTab1 = new QLabel("Tab1 contents", tabWidget);
    QLabel*      labelTab2 = new QLabel("Tab2 contents", tabWidget);
    tabWidget->addTab(labelTab1, "Tab1");
    tabWidget->addTab(labelTab2, "Tab2");
    tabDock->setWidget(tabWidget);
    mainWindow.addDockWidget(Qt::BottomDockWidgetArea, tabDock);

    QMenu*   fileMenu = mainWindow.menuBar()->addMenu("File");
    QAction* fileItem = fileMenu->addAction("File Item");
    QObject::connect(fileItem, &QAction::triggered, [&]() {
        QMessageBox::information(&mainWindow, "Menu Item Selected", "You selected File Item");
    });

    QMenu*   customMenu = mainWindow.menuBar()->addMenu("Custom");
    QAction* customItem = customMenu->addAction("Custom Item");
    QObject::connect(customItem, &QAction::triggered, [&]() {
        QMessageBox::information(&mainWindow, "Menu Item Selected", "You selected Custom Item");
    });

    mainWindow.statusBar()->showMessage("This is the status bar text");

    mainWindow.show();

    return app.exec();
}
