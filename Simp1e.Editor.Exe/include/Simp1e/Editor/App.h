#pragma once

#include <QApplication>

#include "Fonts.h"
#include "Stylesheet.h"

namespace Simp1e::Editor {

    class App : public QApplication {
        int _fakeArgc;

    public:
        App() : QApplication(_fakeArgc, nullptr) {
            setStyle("Fusion");
            LoadFonts();
            setStyleSheet(GetStylesheet());
        }
    };
}
