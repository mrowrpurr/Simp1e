#pragma once

#include <string_format.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QtGlobal>

#include "../IApp.h"

namespace Simp1e::Editor::Windows {

    class DataRecordBrowser : public QWidget {
        Q_OBJECT

        IApp* _app;

#pragma region Widget Variables
        QVBoxLayout* _layout;
        QLabel       _label;
#pragma endregion

    public:
        DataRecordBrowser(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

    private:
#pragma region Widget Setup
        void IDs() { setObjectName("DataRecordBrowser"); }

        void Layout() {
            _layout = new QVBoxLayout();
            _layout->addWidget(&_label);
            setLayout(_layout);
        }

        void Configure() {
            setWindowTitle("Data Record Browser");
            setMinimumSize(400, 400);
            _label.setText(
                string_format("There are {} data records.", _app->GetDataStore().GetRecordCount())
                    .c_str()
            );
        }

        void Events() {}
#pragma endregion

#pragma region Event Handlers
#pragma endregion

#pragma region Private Functions
#pragma endregion
    };
}
