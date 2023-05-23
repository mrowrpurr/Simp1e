#include <QApplication>
#include <QMessageBox>
#include <QPushButton>
#include <QWidget>

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    QWidget      window;
    QPushButton  button("Hello world!", &window);
    button.setToolTip("A tooltip");
    button.resize(200, 100);

    QObject::connect(&button, &QPushButton::clicked, &app, [&]() {
        QMessageBox::information(&window, "Hello", "Hello world!");
    });

    window.resize(250, 150);
    window.setWindowTitle("Simple example");
    window.show();
    return app.exec();
}
