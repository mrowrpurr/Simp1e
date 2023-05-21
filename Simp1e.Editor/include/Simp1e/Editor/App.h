#pragma once

#include <QApplication>
#include <QFontDatabase>
#include <QIcon>

#include "Strings.h"
#include "Windows/Q_DataFilesSelector.h"

namespace Simp1e::Editor {

    class App {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app{argc, argv};

        void LoadFont(const std::string& fontName) {
            int fontId = QFontDatabase::addApplicationFont(QString::fromStdString(fontName));
            if (fontId == -1) {
                throw std::runtime_error("Failed to load font: " + fontName);
            }
            auto  family = QFontDatabase::applicationFontFamilies(fontId).at(0);
            QFont font{family};
            app.setFont(font);
        }

    public:
        int Run(int argc, char* argv[]) {
            LoadFont(":/Fonts/fredericka-the-great.regular.ttf");
            LoadFont(":/Fonts/yoster-island.regular.ttf");
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
