#pragma once

#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <functional>
#include <string>

namespace Simp1e::Editor {

    class DataFileLoaderTestWindow : public QWidget {
        std::function<void(std::string)> _onDataFileLoad;
        QVBoxLayout*                     _layout;
        QLineEdit                        _txt_Path;
        QPushButton                      _btn_Load{"Load Data File"};

    public:
        DataFileLoaderTestWindow(QWidget* parent = nullptr) {
            setObjectName("DataFileLoaderTestWindow");
            setWindowTitle("Data File Loader");
            Layout();
            Styles();
            Events();
        }

        void OnDataFileLoad(std::function<void(std::string)> onDataFileLoad) { _onDataFileLoad = onDataFileLoad; }

    private:
        void Layout() {
            _layout = new QVBoxLayout(this);
            _layout->addWidget(&_txt_Path);
            _layout->addWidget(&_btn_Load);
        }

        void Styles() { _txt_Path.setText(":/Data/ECS.Data.Example1.json"); }

        void Events() {
            QObject::connect(&_btn_Load, &QPushButton::clicked, [this]() {
                _onDataFileLoad(_txt_Path.text().toStdString());
            });
        }
    };
}
