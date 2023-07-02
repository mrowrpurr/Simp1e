#pragma once

#include <Simp1e/DefineComponentType.h>

#include <QMainWindow>
#include <memory>

namespace Simp1e {

    class QMainWindowComponent {
        std::unique_ptr<QMainWindow> _mainWindow;

    public:
        DEFINE_COMPONENT_TYPE("QMainWindow")

        QMainWindowComponent() : QMainWindowComponent(nullptr) {}
        QMainWindowComponent(const char* title) : _mainWindow(std::make_unique<QMainWindow>()) {
            _mainWindow->setWindowTitle(title);
            _mainWindow->show();
        }

        QMainWindow* GetQMainWindow() const { return _mainWindow.get(); }
    };
}
