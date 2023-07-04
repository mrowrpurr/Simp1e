#pragma once

#include <Simp1e/DefineComponentType.h>

#include <QMainWindow>
#include <QVBoxLayout>
#include <memory>

namespace Simp1e {

    class QMainWindowComponent {
        std::unique_ptr<QMainWindow> _mainWindow;

    public:
        DEFINE_COMPONENT_TYPE("QMainWindow")

        QMainWindowComponent() : QMainWindowComponent(nullptr) {}
        QMainWindowComponent(const char* title) : _mainWindow(std::make_unique<QMainWindow>()) {
            _mainWindow->setWindowTitle(title);
            auto* centralWidget = new QWidget();
            centralWidget->setLayout(new QVBoxLayout());
            _mainWindow->setCentralWidget(centralWidget);
            _mainWindow->show();
        }

        QMainWindow* GetQMainWindow() const { return _mainWindow.get(); }
        QWidget*     GetCentralQWidget() const { return _mainWindow->centralWidget(); }
    };
}
