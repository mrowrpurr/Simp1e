#pragma once

#include <Simp1e/Data/Record.h>
#include <Simp1e/Data/Types/ImageRecord.h>
#include <_Log_.h>
#include <string_format.h>

#include <QGroupBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "../IApp.h"

namespace Simp1e::Editor::Windows {

    class DataRecordPreviewWindow : public QWidget {
        Q_OBJECT

        IApp*         _app;
        Data::Record* _record;

#pragma region Widget Variables
        QVBoxLayout* _layout_Window;
        QLabel       _lbl_Title;
        QGroupBox*   _gbx_Preview;
#pragma endregion

    public:
        DataRecordPreviewWindow(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

        void SetRecord(Data::Record* record) {
            _record = record;
            _lbl_Title.setText(
                string_format("ID: {}\nType: {}", record->GetFullIdentifier(), record->GetType())
                    .c_str()
            );
            RenderRecordPreview();
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
        void RenderImagePreview() {
            if (!_record) return;
            Data::Types::ImageRecord imageRecord{_record};
            auto                     imagePath = imageRecord.GetImagePath();
            if (!imagePath) {
                _Log_("No image path");
                return;
            } else {
                _Log_("Image path: {}", imagePath.value());
            }
            auto image =
                QImage{imagePath.value().c_str()}.scaledToHeight(256, Qt::SmoothTransformation);
            if (image.isNull()) return;

            _gbx_Preview = new QGroupBox();
            _gbx_Preview->setTitle("Image Preview");
            auto groupBoxLayout = new QVBoxLayout();
            auto imageLabel     = new QLabel();
            imageLabel->setPixmap(QPixmap::fromImage(image));
            groupBoxLayout->addWidget(imageLabel);
            _gbx_Preview->setLayout(groupBoxLayout);
            _layout_Window->addWidget(_gbx_Preview);
        }

        void ResetPreview() {
            if (_gbx_Preview) {
                _layout_Window->removeWidget(_gbx_Preview);
                delete _gbx_Preview;
                _gbx_Preview = nullptr;
            }
        }

        void RenderRecordPreview() {
            ResetPreview();
            if (!_record) return;
            auto type = std::string{_record->GetType()};
            if (type == "image") RenderImagePreview();
            else _Log_("Unknown type: {}", type);
        }
#pragma endregion
    };
}
