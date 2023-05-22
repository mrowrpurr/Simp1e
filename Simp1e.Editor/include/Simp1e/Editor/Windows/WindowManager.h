#pragma once

#include <Simp1e/Data/Record.h>
#include <string_format.h>

#include <QMessageBox>
#include <memory>

#include "../IApp.h"
#include "DataFilesSelectorWindow_Q.h"
#include "DataRecordBrowserWindow_Q.h"
#include "DataRecordPreviewWindow_Q.h"
#include "MapViewWindow_Q.h"

namespace Simp1e::Editor::Windows {

    class WindowManager {
        IApp*                                             _app;
        std::unique_ptr<Windows::DataFilesSelectorWindow> _dataFilesSelectorWindow;
        std::unique_ptr<Windows::DataRecordBrowserWindow> _dataRecordBrowser;
        std::unique_ptr<Windows::DataRecordPreviewWindow> _dataRecordPreview;
        std::unique_ptr<Windows::MapViewWindow>           _mapViewWindow;

    public:
        WindowManager(IApp* app) : _app(app) {}

        void ShowDataFilesSelector() {
            if (!_dataFilesSelectorWindow)
                _dataFilesSelectorWindow = std::make_unique<Windows::DataFilesSelectorWindow>(_app);
            if (!_dataFilesSelectorWindow->isVisible()) _dataFilesSelectorWindow->show();
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
            if (!_mapViewWindow->isVisible()) _mapViewWindow->show();
        }
        void CloseMapView() { _mapViewWindow->close(); }

        // TODO change all the functins to end with *Window

        void ShowDataRecordPreviewWindow() {
            if (!_dataRecordPreview)
                _dataRecordPreview = std::make_unique<Windows::DataRecordPreviewWindow>(_app);
            if (!_dataRecordPreview->isVisible()) _dataRecordPreview->show();
        }
        void CloseDataRecordPreviewWindow() { _dataRecordPreview->close(); }

        void ShowWindowForRecord(Data::Record* dataRecord) {
            auto type = std::string{dataRecord->GetType()};
            if (type == "gridmap") {
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

        void ShowRecordPreview(Data::Record* dataRecord) {
            ShowDataRecordPreviewWindow();
            _dataRecordPreview->SetRecord(dataRecord);
        }
    };
}
