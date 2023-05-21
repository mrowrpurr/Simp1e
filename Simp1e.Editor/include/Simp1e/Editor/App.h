#pragma once

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QIcon>

#include "IApp.h"
#include "Windows/WindowManager.h"

namespace Simp1e::Editor {

    class App : public IApp {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app{argc, argv};

        Windows::WindowManager _windowManager{this};

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
            _windowManager.ShowDataFilesSelector();
            app.exec();
            return 0;
        }

        void StartUpUsingDataFiles(const std::vector<std::string>& dataFilePaths) override {
            _windowManager.CloseDataFilesSelector();
            QWidget* newWidget = new QWidget();
            newWidget->setWindowTitle("Hello...");
            newWidget->show();
        }
    };
}
