#pragma once

#include <memory>

#include "../IApp.h"
#include "DataFilesSelector_Q.h"

namespace Simp1e::Editor::Windows {

    class WindowManager {
        IApp*                                             _app;
        std::unique_ptr<Windows::DataFilesSelectorWindow> _dataFilesSelectorWindow;

    public:
        WindowManager(IApp* app) : _app(app) {}

        void ShowDataFilesSelector() {
            if (!_dataFilesSelectorWindow)
                _dataFilesSelectorWindow = std::make_unique<Windows::DataFilesSelectorWindow>(_app);
            _dataFilesSelectorWindow->show();
        }

        void CloseDataFilesSelector() { _dataFilesSelectorWindow->close(); }
    };
}
