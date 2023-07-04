#pragma once

#include <QApplication>

namespace Simp1e {

    class QSimp1eApp : public QApplication {
        int _mockArgcForQApplication = 0;

    public:
        QSimp1eApp(int& argc, char** argv) : QApplication(argc, argv) { setStyle("Fusion"); }
        QSimp1eApp() : QSimp1eApp(_mockArgcForQApplication, nullptr) {}
    };
}
