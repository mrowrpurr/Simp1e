#pragma once

#include <QWidget>

namespace Simp1e::Editor {

    class PrefabListWindow : public QWidget {
    public:
        PrefabListWindow(QWidget* parent = nullptr) {
            setObjectName("PrefabListWindow");
            setWindowTitle("Prefab List");
        }
    };
}
