#pragma once

#include <Simp1e/Data/Record.h>
#include <Simp1e/Data/RecordData.h>
#include <string_format.h>

#include <QImage>
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
        int                       _cellSize{256};
        int                       _padding{0};
        QVBoxLayout*              _layout_Window;
        QLabel                    _lbl_Title;
        MapView::MapGraphicsScene _mapGraphicsScene;
        MapView::MapGraphicsView  _mapGraphicsView;
#pragma endregion

    public:
        MapViewWindow(IApp* app) : _app(app), QWidget(nullptr) {
            IDs();
            Layout();
            Events();
            Configure();
        }

        void SetMap(Data::Record* record) {
            _mapRecord = std::make_unique<Data::MapRecord>(&_app->GetDataStore(), record);
            _lbl_Title.setText(string_format(
                                   "Map: {} ({}x{})", _mapRecord->GetID(), _mapRecord->GetCols(),
                                   _mapRecord->GetRows()
            )
                                   .c_str());
            RenderMap();
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

        void Configure() { _mapGraphicsView.setScene(&_mapGraphicsScene); }

        void Events() {}
#pragma endregion

#pragma region Event Handlers
#pragma endregion

#pragma region Private Functions
        void AddImagesToRectFromMapDefinition(
            MapView::MapCellGraphicsRectItem* rect, Data::MapGridCell& cell
        ) {
            if (!cell.HasLayers()) return;
            for (int i = 0; i < cell.GetLayerCount(); i++) {
                auto imagePath = cell.GetLayerImagePath(i);
                if (imagePath) rect->AddImage(QImage{imagePath->string().c_str()});
            }
        }

        void RenderMap() {
            _mapGraphicsScene.clear();
            if (!_mapRecord->HasContent()) return;
            for (int i = 0; i < _mapRecord->GetCols(); i++) {
                for (int j = 0; j < _mapRecord->GetRows(); j++) {
                    auto* rect = new MapView::MapCellGraphicsRectItem(
                        i * _cellSize, j * _cellSize, _cellSize - _padding, _cellSize - _padding
                    );
                    auto cell = _mapRecord->GetCellAt(i, j);
                    if (cell) AddImagesToRectFromMapDefinition(rect, *cell);
                    _mapGraphicsScene.addItem(rect);
                }
            }
        }
#pragma endregion
    };
}
