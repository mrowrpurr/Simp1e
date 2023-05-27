#pragma once

#include <QApplication>

#include "Scene.h"
#include "View.h"

namespace Prototyping::Qt {

    class App {
        int            _appArgc = 0;
        QApplication   app{_appArgc, nullptr};
        QGraphicsView* _view;

    public:
        App() {
#ifdef _WIN32
            app.setStyle("Fusion");
#endif
        }

        QApplication*  GetQApplication() { return &app; }
        QGraphicsView* GetQGraphicsView() { return _view; }

        App& SetView(QGraphicsView* view) {
            _view = view;
            return *this;
        }

        App& SetScene(QGraphicsScene* scene) {
            if (!_view) _view = new View();
            _view->setScene(scene);
            return *this;
        }

        int Run() {
            if (!_view) {
                _view = new View();
                _view->setScene(new Scene());
            }
            _view->show();
            return app.exec();
        }
    };
}
