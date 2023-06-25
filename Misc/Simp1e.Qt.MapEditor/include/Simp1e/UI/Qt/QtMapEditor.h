#pragma once

#include <Simp1e/UI/UIMapEditor.h>

#include <QMainWindow>

namespace Simp1e::UI::Qt {

    class QtMapEditor : public QMainWindow, public UIMapEditor {
    public:
        QtMapEditor() : QMainWindow() { setWindowTitle("Simp1e Map Editor"); }
    };
}
