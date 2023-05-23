#pragma once

#include <Simp1e/Data/Record.h>
#include <string_format.h>

#include <QDockWidget>
#include <QMainWindow>
#include <QMessageBox>
#include <memory>

#include "../IApp.h"
#include "DataFilesSelectorWindow_Q.h"
#include "DataRecordBrowserWindow_Q.h"
#include "DataRecordPreviewWindow_Q.h"
#include "MapViewWindow_Q.h"

namespace Simp1e::Editor::Windows {

    class WindowManager {
        IApp*       _app;
        QMainWindow _mainWindow;

        std::unique_ptr<Windows::DataFilesSelectorWindow> _dataFilesSelectorWindow;
        std::unique_ptr<Windows::DataRecordBrowserWindow> _dataRecordBrowser;
        std::unique_ptr<Windows::DataRecordPreviewWindow> _dataRecordPreview;
        std::unique_ptr<Windows::MapViewWindow>           _mapViewWindow;

        QDockWidget* _recordBrowserDockWidget;
        QDockWidget* _recordPreviewDockWidget;
        QDockWidget* _mapViewDockWidget;

        void SetupMainApplication() {
            _dataRecordBrowser.reset(new Windows::DataRecordBrowserWindow(_app));
            _dataRecordPreview.reset(new Windows::DataRecordPreviewWindow(_app));
            _mapViewWindow.reset(new Windows::MapViewWindow(_app));

            _recordBrowserDockWidget = new QDockWidget("Data Records");
            _recordBrowserDockWidget->setWidget(_dataRecordBrowser.get());

            _recordPreviewDockWidget = new QDockWidget("Data Record Preview");
            _recordPreviewDockWidget->setWidget(_dataRecordPreview.get());

            _mapViewDockWidget = new QDockWidget("Map View");
            _mapViewDockWidget->setWidget(_mapViewWindow.get());

            _mainWindow.addDockWidget(
                Qt::DockWidgetArea::LeftDockWidgetArea, _recordBrowserDockWidget
            );
            _mainWindow.addDockWidget(
                Qt::DockWidgetArea::RightDockWidgetArea, _recordPreviewDockWidget
            );
            _mainWindow.addDockWidget(Qt::DockWidgetArea::BottomDockWidgetArea, _mapViewDockWidget);
        }

        void ShowStartupScreen() {
            _dataFilesSelectorWindow.reset(new Windows::DataFilesSelectorWindow(_app));
            _dataFilesSelectorWindow->show();
        }
        void BootUpMainApplication() {
            SetupMainApplication();
            _dataRecordBrowser->ReloadRecords();
            _mainWindow.show();
            _dataFilesSelectorWindow->close();
        }

    public:
        WindowManager(IApp* app) : _app(app) {}

        void ShowDataFilesSelector() { ShowStartupScreen(); }
        void ShowDataRecordBrowser() { BootUpMainApplication(); }

        void ShowRecordPreview(Data::Record* dataRecord) {
            _dataRecordPreview->SetRecord(dataRecord);

            auto type = std::string{dataRecord->GetType()};
            if (type == "gridmap") {
                _mapViewWindow->SetMap(dataRecord);
                _mapViewDockWidget->show();
                _mapViewWindow->show();
            }
        }
    };
}
