#pragma once

#include <QDockWidget>
#include <QMainWindow>

namespace Simp1e::Editor {

    class AppWindow : public QMainWindow {
    public:
        AppWindow(QWidget* parent = nullptr) {
            setWindowTitle("Simp1e");
            setObjectName("AppWindow");
        }

        void DockWidget(QWidget* widget, Qt::DockWidgetArea area, bool floating = false) {
            auto dockWidget = new QDockWidget(this);
            dockWidget->setWidget(widget);
            dockWidget->setFloating(floating);
            dockWidget->setWindowTitle(widget->windowTitle());
            addDockWidget(area, dockWidget);
        }
    };
}
