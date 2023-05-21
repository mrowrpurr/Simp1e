#pragma once

#include <QApplication>
#include <QIcon>

#include "Strings.h"
#include "Windows/Q_DataFilesSelector.h"

namespace Simp1e::Editor {

    class App {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app{argc, argv};

    public:
        int Run(int argc, char* argv[]) {
            QIcon item{":icon.png"};
            app.setWindowIcon(item);
#ifdef _WIN32
            app.setStyle("fusion");
#endif
            QFile file(":/Editor.qss");
            file.open(QFile::ReadOnly);
            QString styleSheet = QLatin1String(file.readAll());
            app.setStyleSheet(styleSheet);
            DataFilesSelectorWindow window;
            window.setWindowTitle(Strings::WindowTitle);
            window.setMinimumWidth(800);
            window.show();
            app.exec();
            return 0;
        }
    };
}
