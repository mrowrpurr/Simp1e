#pragma once

#include <QWidget>

namespace Simp1e::Editor {

    class SystemListWindow : public QWidget {
    public:
        SystemListWindow(QWidget* parent = nullptr) {
            setObjectName("SystemListWindow");
            setWindowTitle("System List");
        }
    };
}
