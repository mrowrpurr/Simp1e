#pragma once

#include <Simp1e/Data/Record.h>
#include <string_format.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "../IApp.h"

namespace Simp1e::Editor::Windows {

    class DataRecordPreviewWindow : public QWidget {
        Q_OBJECT

        IApp* _app;

#pragma region Widget Variables
        QVBoxLayout* _layout_Window;
        QLabel       _lbl_Title;
#pragma endregion

    public:
        DataRecordPreviewWindow(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

        void SetRecord(Data::Record* record) {
            _lbl_Title.setText(
                string_format("ID: {}\nType: {}", record->GetFullIdentifier(), record->GetType())
                    .c_str()
            );
        }

    private:
#pragma region Widget Setup
        void IDs() { setObjectName("DataRecordBrowser"); }

        void Layout() {
            _layout_Window = new QVBoxLayout();
            _layout_Window->addWidget(&_lbl_Title);
            setLayout(_layout_Window);
        }

        void Configure() {}

        void Events() {}
#pragma endregion

#pragma region Event Handlers
#pragma endregion

#pragma region Private Functions
#pragma endregion
    };
}
