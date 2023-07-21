#pragma once

#include <QWidget>

namespace Simp1e::Editor {

    class ComponentListWindow : public QWidget {
    public:
        ComponentListWindow(QWidget* parent = nullptr) {
            setObjectName("ComponentListWindow");
            setWindowTitle("Component List");
        }
    };
}
