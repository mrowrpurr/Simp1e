#pragma once

#include <Simp1e/Data/JsonDataFile.h>
#include <Simp1e/Data/JsonDataStore.h>
#include <string_format.h>

#include <QApplication>
#include <QFile>
#include <QFileSystemWatcher>
#include <QFontDatabase>
#include <QIcon>
#include <QString>
#include <QWidget>
#include <string>
#include <utility>
#include <vector>

#include "IApp.h"
#include "Paths.h"
#include "Windows/WindowManager.h"

namespace Simp1e::Editor {

    class App : public IApp {
        int                 argc = 0;
        char**              argv = nullptr;
        QApplication        app{argc, argv};
        QFileSystemWatcher* _qssChangeWatcher;

        Data::JsonDataStore    _dataStore;
        Data::JsonDataFile     _activeDataFile;  // Not used yet...
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

        void ReloadStylesheet() {
            QFile file(Paths::Development::QssFile);
            file.open(QFile::ReadOnly);
            QString styleSheet = QLatin1String(file.readAll());
            app.setStyleSheet(styleSheet);
        }

        void WatchForQssChangesAndReloadStylesheet() {
            _qssChangeWatcher = new QFileSystemWatcher();
            _qssChangeWatcher->addPath(Paths::Development::QssFile.string().c_str());
            QObject::connect(_qssChangeWatcher, &QFileSystemWatcher::fileChanged, [this]() {
                ReloadStylesheet();
            });
        }

    public:
        Data::JsonDataStore& GetDataStore() override { return _dataStore; }
        Data::JsonDataFile&  GetActiveDataFile() override { return _activeDataFile; }

        int Run(int argc, char* argv[]) {
            LoadFont(":/Fonts/fredericka-the-great.regular.ttf");
            LoadFont(":/Fonts/yoster-island.regular.ttf");
            QIcon item{":icon.png"};
            app.setWindowIcon(item);
#ifdef _WIN32
            app.setStyle("fusion");
#endif
            ReloadStylesheet();
            WatchForQssChangesAndReloadStylesheet();
            _windowManager.ShowDataFilesSelector();
            app.exec();
            return 0;
        }

        void StartUpUsingDataFiles(const std::vector<std::pair<std::string, bool>>& dataFilePaths
        ) override {
            for (auto& [path, active] : dataFilePaths) {
                Data::JsonDataFile dataFile{path};
                _dataStore.InsertDataFile(dataFile);
                if (active) _activeDataFile = Data::JsonDataFile{path};
            }
            _windowManager.ShowDataRecordBrowser();
        }

        void ShowRecordPreview(Data::Record* dataRecord) override {
            _windowManager.ShowRecordPreview(dataRecord);
        }
    };
}
