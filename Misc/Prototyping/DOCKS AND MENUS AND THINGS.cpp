#include <QApplication>
#include <QDockWidget>
#include <QLabel>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QProxyStyle>
#include <QStatusBar>
#include <QStyleOption>
#include <QStylePainter>
#include <QTabWidget>
#include <QToolBar>
#include <QVBoxLayout>

class MyStyle : public QProxyStyle {
public:
    void drawControl(ControlElement element, const QStyleOption* option, QPainter* painter, const QWidget* widget)
        const {
        if (element == CE_Splitter) {
            painter->save();
            painter->setPen(QPen(Qt::red, 3));  // Change the color and size of the splitter line here
            if (option->state & State_Horizontal) {
                painter->drawLine(option->rect.topLeft(), option->rect.topRight());
            } else {
                painter->drawLine(option->rect.topLeft(), option->rect.bottomLeft());
            }
            painter->restore();
        } else {
            QProxyStyle::drawControl(element, option, painter, widget);
        }
    }
};

int main(int argc, char** argv) {
    QApplication app(argc, argv);
    app.setStyle("Fusion");
    app.setStyleSheet(R"(
    
QMainWindow::separator {
    background-color: green;
    width: 4px;
    border: none;
}

QDockWidget {
    /* Set a border for the QDockWidget */
    border: 1px solid lightgray;

    /* Set custom icons for the close and float buttons */
    titlebar-close-icon: url(close.png);
    titlebar-normal-icon: url(undock.png);
}

QDockWidget::title {
    /* Align the text to the left and set a background color */
    text-align: left;
    background: lightgray;
    padding-left: 5px;
}

QDockWidget::close-button, QDockWidget::float-button {
    /* Set the background color for the buttons */
    background: darkgray;
    padding: 0px;
}

QDockWidget::close-button:hover, QDockWidget::float-button:hover {
    /* Set the background color for the buttons on hover */
    background: gray;
}

QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {
    /* Adjust the padding when the buttons are pressed */
    padding: 1px -1px -1px 1px;
}


    )");

    QMainWindow mainWindow;
    mainWindow.setStyle(new MyStyle());
    mainWindow.setWindowTitle("This is the main window title");

    QWidget* widgetA = new QWidget;
    QLabel*  labelA  = new QLabel("Widget A", widgetA);

    mainWindow.setCentralWidget(widgetA);

    QDockWidget* topDock = new QDockWidget("Widget B", &mainWindow);
    QLabel*      labelB  = new QLabel("Widget B", topDock);
    topDock->setWidget(labelB);
    topDock->setStyleSheet("QDockWidget { border: 1px solid red; }");

    mainWindow.addDockWidget(Qt::TopDockWidgetArea, topDock);

    QDockWidget* bottomDock = new QDockWidget("Widget C", &mainWindow);
    QLabel*      labelC     = new QLabel("Widget C", bottomDock);
    bottomDock->setWidget(labelC);
    mainWindow.addDockWidget(Qt::BottomDockWidgetArea, bottomDock);
    QLabel* label = new QLabel("CUSTON Header Text", bottomDock);
    label->setStyleSheet("color: orange; font-size: 14pt; font-weight: bold;");
    bottomDock->setTitleBarWidget(label);

    QDockWidget* tabbedDock  = new QDockWidget("Already tabbed", &mainWindow);
    QLabel*      labelTabbed = new QLabel("Already tabbed", tabbedDock);
    tabbedDock->setWidget(labelTabbed);
    mainWindow.tabifyDockWidget(bottomDock, tabbedDock);

    QDockWidget* leftDock = new QDockWidget("Widget D", &mainWindow);
    QLabel*      labelD   = new QLabel("Widget D", leftDock);
    leftDock->setWidget(labelD);
    mainWindow.addDockWidget(Qt::LeftDockWidgetArea, leftDock);
    leftDock->setStyleSheet("QDockWidget { border: 1px solid blue; }");

    QDockWidget* rightDock = new QDockWidget("Widget E", &mainWindow);
    QLabel*      labelE    = new QLabel("Widget E", rightDock);
    rightDock->setWidget(labelE);
    mainWindow.addDockWidget(Qt::RightDockWidgetArea, rightDock);
    rightDock->setStyleSheet("QDockWidget { border: 1px solid green; }");

    QDockWidget* tabDock   = new QDockWidget("Widget F", &mainWindow);
    QTabWidget*  tabWidget = new QTabWidget(tabDock);
    QLabel*      labelTab1 = new QLabel("Tab1 contents", tabWidget);
    QLabel*      labelTab2 = new QLabel("Tab2 contents", tabWidget);
    tabWidget->addTab(labelTab1, "Tab1");
    tabWidget->addTab(labelTab2, "Tab2");
    tabDock->setWidget(tabWidget);
    mainWindow.addDockWidget(Qt::BottomDockWidgetArea, tabDock);

    QMenu*   fileMenu = mainWindow.menuBar()->addMenu("File");
    QAction* fileItem = fileMenu->addAction("File Item");
    QObject::connect(fileItem, &QAction::triggered, [&]() {
        QMessageBox::information(&mainWindow, "Menu Item Selected", "You selected File Item");
    });

    QMenu*   customMenu = mainWindow.menuBar()->addMenu("Custom");
    QAction* customItem = customMenu->addAction("Custom Item");
    QObject::connect(customItem, &QAction::triggered, [&]() {
        QMessageBox::information(&mainWindow, "Menu Item Selected", "You selected Custom Item");
    });

    mainWindow.statusBar()->showMessage("This is the status bar text");

    mainWindow.show();

    return app.exec();
}
