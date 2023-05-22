#pragma once

#include <Simp1e/Data/Record.h>
#include <string_format.h>

#include <QMessageBox>
#include <memory>

#include "../IApp.h"
#include "DataFilesSelectorWindow_Q.h"
#include "DataRecordBrowserWindow_Q.h"
#include "MapViewWindow_Q.h"

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

        void ShowMapView(Data::Record* dataRecord) {
            if (!_mapViewWindow)
                _mapViewWindow = std::make_unique<Windows::MapViewWindow>(_app, dataRecord);
            _mapViewWindow->show();
        }
        void CloseMapView() { _mapViewWindow->close(); }

        void ShowWindowForRecord(Data::Record* dataRecord) {
            auto type = dataRecord->GetType();
            if (strcmp(type, "gridmap") == 0) {
                ShowMapView(dataRecord);
            } else {
                QMessageBox msgBox;
                msgBox.setText(string_format("Unsupported record type: '{}'", type).c_str());
                msgBox.setInformativeText("This record type is not supported by the editor.");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.setDefaultButton(QMessageBox::Ok);
                msgBox.exec();
            }
        }
    };
}
