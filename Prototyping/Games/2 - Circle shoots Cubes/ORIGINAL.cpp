#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <functional>
#include <map>
#include <random>







int main(int argc, char* argv[]) {
    int z = 420;
    int x = 69;

    QApplication app{argc, argv};

    GridMap map(10, 15);
    // map.SetTileLines(true);
    // map.MoveCircleTo(5, 5);
    // map.AddCube(1, 2);
    // map.AddCube(8, 7);
    // map.AddCube(3, 8);

    auto scene = map.getScene();

    KeyPressGraphicsView view(scene);
    view.show();

    // map.Redraw();

    app.exec();

    return 0;
}
