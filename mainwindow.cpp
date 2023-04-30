#include "mainwindow.h"

#include <QPushButton>


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QPushButton* button = new QPushButton("Hello, World!", this);
    setCentralWidget(button);
}