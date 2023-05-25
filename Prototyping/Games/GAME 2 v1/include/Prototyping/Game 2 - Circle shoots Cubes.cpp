#include <QApplication>

#include "Prototyping/GameGraphicsView.h"
#include "Prototyping/GameMapForCircleAndCubes.h"

int main(int argc, char* argv[]) {
    QApplication                          app{argc, argv};
    Prototyping::GameMapForCircleAndCubes map{10, 20};
    Prototyping::GameGraphicsView         view;
    view.SetScene(map.GetScene());
    // Move the view handling into the game
    view.OnUpKey([&map]() { map.MoveCircleUp(); });
    view.OnDownKey([&map]() { map.MoveCircleDown(); });
    view.OnLeftKey([&map]() { map.MoveCircleLeft(); });
    view.OnRightKey([&map]() { map.MoveCircleRight(); });
    view.setFixedSize(map.GetWidth() + 10, map.GetHeight() + 10);
    view.show();
    return app.exec();
}
