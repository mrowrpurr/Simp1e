#pragma once

// Move this somewhere...
#if defined(SIMP1E_TESTING)
    #define SHOW_WIDGET(qwidget)
    #define RUN_APP(app)
#else
    #define SHOW_WIDGET(qwidget) qwidget.show()
    #define RUN_APP(app) app.exec()
#endif
//...

#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

int TheStuffFromMainGoesHere(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QWidget      window;
    QPushButton  button("Hello from a .h Header File!", &window);
    button.setToolTip("A tooltip");
    button.resize(200, 100);

    QObject::connect(&button, &QPushButton::clicked, &app, [&]() {
        QMessageBox::information(&window, "Hello", "Hello world!");
    });

    window.resize(250, 150);
    window.setWindowTitle("Simple example");

    SHOW_WIDGET(window);
    RUN_APP(app);

    return true;
}
