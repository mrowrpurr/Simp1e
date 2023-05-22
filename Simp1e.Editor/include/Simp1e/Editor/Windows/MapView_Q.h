#pragma once

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "../IApp.h"

namespace Simp1e::Editor::Windows {

    class MapViewWindow : public QWidget {
        Q_OBJECT

        IApp* _app;

#pragma region Widget Variables
        QVBoxLayout* _layout_Window;
        QLabel       _lbl_HelloWorld{"Hello from this window"};
#pragma endregion

    public:
        MapViewWindow(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

    private:
#pragma region Widget Setup
        void IDs() { setObjectName("DataRecordBrowser"); }

        void Layout() {
            _layout_Window = new QVBoxLayout(this);
            _layout_Window->addWidget(&_lbl_HelloWorld);
            setLayout(_layout_Window);
        }

        void Configure() { setWindowTitle("Wassup?"); }

        void Events() {}
#pragma endregion

#pragma region Event Handlers
#pragma endregion

#pragma region Private Functions
#pragma endregion
    };
}
