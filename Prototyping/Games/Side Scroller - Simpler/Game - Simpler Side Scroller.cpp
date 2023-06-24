#include <_Log_.h>
_LogToFile_("SimpleSideScroller.log");
///////////////////////////////////////////

#include <Simp1e/Game.h>
#include <Simp1e/Size.h>

#include <QApplication>
#include <QDebug>
#include <QGraphicsColorizeEffect>
#include <QGraphicsSvgItem>
#include <QTimer>

#include "SideScrollerEntities.h"
#include "SideScrollerGraphicsScene.h"
#include "SideScrollerGraphicsView.h"
#include "SideScrollerImages.h"
#include "SideScrollerSystems.h"


using namespace Simp1e;
using namespace Simp1e::ECS;
using namespace Simp1e::SideScroller;

Game   game;
Size   levelSize{10000, 1000};
QTimer mainLoopTimer;

void GameLoop() { game.Update(); }

void PlayWithScrollingBackgrounds(QGraphicsScene& scene) {
    // auto path = "C:/Code/mrowrpurr/StockImages/shutterstock_1382713007.svg";
    // auto path = "C:/Code/mrowrpurr/StockImages/shutterstock_1575470704.svg";
    auto path = "C:/Code/mrowrpurr/StockImages/shutterstock_1575470704 - sky removed.svg";
    auto svg  = new QGraphicsSvgItem(path);

    // colorize
    auto effect = new QGraphicsColorizeEffect();
    effect->setColor(QColor(255, 0, 255));
    svg->setGraphicsEffect(effect);

    svg->setPos(0, 0);
    qDebug() << "Adding svg with bounding rect of" << svg->boundingRect() << "to scene";
    scene.addItem(svg);
}

int main(int argc, char* argv[]) {
    /*
        Qt UI
    */

    QApplication app(argc, argv);
    app.setStyle("fusion");

    SideScrollerGraphicsScene scene;  // (game.Events());
    scene.setSceneRect(0, 0, levelSize.width(), levelSize.height());

    SideScrollerGraphicsView view;  // (game.Events());
    view.setWindowTitle("Side Scroller");
    view.setScene(&scene);

    PlayWithScrollingBackgrounds(scene);

    SetupSystems();
    SetupEntities();

    /*
        Start game main() loop and run the application
    */

    QObject::connect(&mainLoopTimer, &QTimer::timeout, &app, GameLoop);
    mainLoopTimer.start(16);
    view.move(0, 0);
    view.show();
    return app.exec();
}
