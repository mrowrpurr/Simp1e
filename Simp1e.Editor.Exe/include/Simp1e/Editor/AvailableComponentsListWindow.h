#pragma once

#include <QWidget>

namespace Simp1e::Editor {

    class AvailableComponentsListWindow : public QWidget {
    public:
        AvailableComponentsListWindow(QWidget* parent = nullptr) {
            setObjectName("AvailableComponentsListWindow");
            setWindowTitle("Available Components");
        }
    };
}
