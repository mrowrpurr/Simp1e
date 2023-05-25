#include <QApplication>
#include <QWidget>

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};
    QWidget      widget;
    widget.setFixedSize(100, 100);
    widget.show();
    return app.exec();
}
