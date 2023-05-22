#pragma once

#include <Simp1e/Data/Record.h>
#include <string_format.h>

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <memory>

#include "../../Data/MapRecord.h"
#include "../IApp.h"
#include "MapView/MapGraphicsScene.h"
#include "MapView/MapGraphicsView.h"

namespace Simp1e::Editor::Windows {

    class MapViewWindow : public QWidget {
        Q_OBJECT

        IApp*                            _app;
        std::unique_ptr<Data::MapRecord> _mapRecord;

#pragma region Widget Variables
        int                       _cellSize{32};
        int                       _padding{4};
        QVBoxLayout*              _layout_Window;
        QLabel                    _lbl_Title;
        MapView::MapGraphicsScene _mapGraphicsScene;
        MapView::MapGraphicsView  _mapGraphicsView;
#pragma endregion

    public:
        MapViewWindow(IApp* app, Data::Record* record)
            : _app(app), _mapRecord(std::make_unique<Data::MapRecord>(record)), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

    private:
#pragma region Widget Setup
        void IDs() { setObjectName("MapViewWindow"); }

        void Layout() {
            _layout_Window = new QVBoxLayout(this);
            _layout_Window->addWidget(&_lbl_Title);
            _layout_Window->addWidget(&_mapGraphicsView);
            setLayout(_layout_Window);
        }

        void Configure() {
            setWindowTitle("Map View");
            _lbl_Title.setText(string_format(
                                   "Map: {} ({}x{})", _mapRecord->GetID(), _mapRecord->GetCols(),
                                   _mapRecord->GetRows()
            )
                                   .c_str());

            _mapGraphicsView.setScene(&_mapGraphicsScene);

            // Add some rows/columns just for testing
            for (int i = 0; i < _mapRecord->GetCols(); i++) {
                for (int j = 0; j < _mapRecord->GetRows(); j++) {
                    auto* rect = new MapView::MapCellGraphicsRectItem(
                        i * _cellSize, j * _cellSize, _cellSize - _padding, _cellSize - _padding
                    );
                    _mapGraphicsScene.addItem(rect);
                }
            }
        }

        void Events() {}
#pragma endregion

#pragma region Event Handlers
#pragma endregion

#pragma region Private Functions
#pragma endregion
    };
}
