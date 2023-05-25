#include <QApplication>

#include "Prototyping/GameGraphicsView.h"
#include "Prototyping/GameMapForCircleAndCubes.h"

int main(int argc, char* argv[]) {
    QApplication                          app{argc, argv};
    Prototyping::GameMapForCircleAndCubes map{10, 20};
    Prototyping::GameGraphicsView         view;
    view.SetScene(map.GetScene());
    map.Redraw();
    view.show();
    return app.exec();
}
