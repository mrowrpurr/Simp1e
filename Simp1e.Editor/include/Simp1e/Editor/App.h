#pragma once

#include <Simp1e/Data/JsonDataStore.h>
#include <string_format.h>

#include <QApplication>
#include <QFile>
#include <QFontDatabase>
#include <QIcon>
#include <QString>
#include <QWidget>
#include <string>
#include <utility>
#include <vector>

#include "IApp.h"
#include "Windows/WindowManager.h"

namespace Simp1e::Editor {

    class App : public IApp {
        int          argc = 0;
        char**       argv = nullptr;
        QApplication app{argc, argv};

        Data::JsonDataStore    _dataStore;
        Data::JsonDataFile     _activeDataFile;
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

        void StartUpUsingDataFiles(const std::vector<std::pair<std::string, bool>>& dataFilePaths
        ) override {
            std::string summary = "SUMMARY: \n";
            for (auto& [path, active] : dataFilePaths) {
                summary += string_format("loading {}\n", path);
                Data::JsonDataFile dataFile{path};
                _dataStore.InsertDataFile(dataFile);
                // if (active) _activeDataFile = dataFile;
            }

            QWidget* newWidget = new QWidget();
            newWidget->setWindowTitle("Next Window!");

            summary +=
                string_format("The data store has {} records\n", _dataStore.GetAllRecords().size());
            for (auto& record : _dataStore.GetAllRecords())
                summary += string_format("{}\n", record->GetFullIdentifier());
            QLabel* label = new QLabel(summary.c_str());
            label->setAlignment(Qt::AlignCenter);
            QVBoxLayout* layout = new QVBoxLayout();
            layout->addWidget(label);
            newWidget->setLayout(layout);
            newWidget->show();
            _windowManager.CloseDataFilesSelector();
        }
    };
}
