#pragma once

#include <QWidget>

namespace Simp1e::Editor {

    class EntityListWindow : public QWidget {
    public:
        EntityListWindow(QWidget* parent = nullptr) {
            setObjectName("EntityListWindow");
            setWindowTitle("Entity List");
        }
    };
}
