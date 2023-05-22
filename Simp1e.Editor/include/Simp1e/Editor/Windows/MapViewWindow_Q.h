#pragma once

#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>

#include "../IApp.h"
#include "MapView/MapGraphicsScene.h"
#include "MapView/MapGraphicsView.h"

namespace Simp1e::Editor::Windows {

    class MapViewWindow : public QWidget {
        Q_OBJECT

        IApp* _app;

#pragma region Widget Variables
        QVBoxLayout*              _layout_Window;
        QLabel                    _lbl_Title{"Map: <name of the map> (10x20)"};
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
            _mapGraphicsView.setScene(&_mapGraphicsScene);

            // Add some rows/columns just for testing
            auto cellSize = 32;
            auto padding  = 0;
            auto rows     = 10;
            auto cols     = 20;
            for (int i = 0; i < cols; i++) {
                for (int j = 0; j < rows; j++) {
                    auto* rect = new MapView::MapCellGraphicsRectItem(
                        i * cellSize, j * cellSize, cellSize - padding, cellSize - padding
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
