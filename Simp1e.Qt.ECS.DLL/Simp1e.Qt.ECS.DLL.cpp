#include <Simp1e/ServiceHostClient.h>

#include <QApplication>
#include <QWidget>


OnSimp1eStart {
    int          argc;
    QApplication a(argc, nullptr);
    QWidget      w;
    w.setWindowTitle("Hello World!");
    w.show();
    a.exec();
}
