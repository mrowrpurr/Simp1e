#include <Simp1e/ServiceHostClient.h>

#include <QApplication>
#include <QWidget>

// Let's have something that listens for ToolbarButton component creation
// and adds them to out window.

// Or just Label first...

OnSimp1eStart {
    int          argc;
    QApplication a(argc, nullptr);
    QWidget      w;
    w.setWindowTitle("Hello World!");
    w.show();
    a.exec();
}
