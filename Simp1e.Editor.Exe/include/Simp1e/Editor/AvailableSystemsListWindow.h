#pragma once

#include <QWidget>

namespace Simp1e::Editor {

    class AvailableSystemsListWindow : public QWidget {
    public:
        AvailableSystemsListWindow(QWidget* parent = nullptr) {
            setObjectName("AvailableSystemsListWindow");
            setWindowTitle("Available Systems");
        }
    };
}
