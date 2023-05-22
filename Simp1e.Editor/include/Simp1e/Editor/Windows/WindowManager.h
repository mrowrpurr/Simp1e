#pragma once

#include <memory>

#include "../IApp.h"
#include "DataFilesSelector_Q.h"
#include "DataRecordBrowser_Q.h"
#include "MapView_Q.h"

namespace Simp1e::Editor::Windows {

    class WindowManager {
        IApp*                                             _app;
        std::unique_ptr<Windows::DataFilesSelectorWindow> _dataFilesSelectorWindow;
        std::unique_ptr<Windows::DataRecordBrowserWindow> _dataRecordBrowser;
        std::unique_ptr<Windows::MapViewWindow>           _mapViewWindow;

    public:
        WindowManager(IApp* app) : _app(app) {}

        void ShowDataFilesSelector() {
            if (!_dataFilesSelectorWindow)
                _dataFilesSelectorWindow = std::make_unique<Windows::DataFilesSelectorWindow>(_app);
            _dataFilesSelectorWindow->show();
        }
        void CloseDataFilesSelector() { _dataFilesSelectorWindow->close(); }

        void ShowDataRecordBrowser() {
            if (!_dataRecordBrowser)
                _dataRecordBrowser = std::make_unique<Windows::DataRecordBrowserWindow>(_app);
            _dataRecordBrowser->show();
        }
        void CloseDataRecordBrowser() { _dataRecordBrowser->close(); }

        void ShowMapView() {
            if (!_mapViewWindow) _mapViewWindow = std::make_unique<Windows::MapViewWindow>(_app);
            _mapViewWindow->show();
        }
        void CloseMapView() { _mapViewWindow->close(); }
    };
}
