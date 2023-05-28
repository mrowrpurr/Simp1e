#pragma once

#include <QDockWidget>
#include <QGuiApplication>
#include <QMainWindow>
#include <QScreen>

#include "../UITileGrid.h"
#include "QtTileGrid.h"

namespace Prototyping::UI::Qt {

    class QtMutlipleTileGrids : public UITileGrid {
        UITileGrid::Config                       _config;
        std::vector<std::unique_ptr<QtTileGrid>> _qtTileGrids;

        QMainWindow _window;

        std::string GetDockNameForRenderingStyle(UITileGrid::RenderingStyle renderingStyle) {
            switch (renderingStyle) {
                case UITileGrid::RenderingStyle::Grid:
                    return "Grid";
                case UITileGrid::RenderingStyle::Isometric:
                    return "Isometric";
                case UITileGrid::RenderingStyle::Hexagons:
                    return "Hexagons";
                case UITileGrid::RenderingStyle::IsometricWithHexagons:
                    return "Isometric with Hexagons";
                default:
                    return "Unknown rendering style";
            }
        }

        void CenterWindow() {
            _window.adjustSize();
            auto* screen   = QGuiApplication::primaryScreen();
            auto  geometry = screen->availableGeometry();
            int   x        = (geometry.width() - _window.width()) / 2;
            int   y        = (geometry.height() - _window.height()) / 2;
            _window.move(x, y);
        }

    public:
        QtMutlipleTileGrids(
            const Config& config, std::vector<UITileGrid::RenderingStyle> renderingStyles
        )
            : _config(config) {
            QDockWidget* firstDock = nullptr;
            for (auto style : renderingStyles) {
                Config gridConfig         = config;
                gridConfig.renderingStyle = style;
                auto* tileGrid            = new QtTileGrid(gridConfig);
                auto* dock = new QDockWidget(GetDockNameForRenderingStyle(style).c_str());
                dock->setWidget(tileGrid->GetWidget());
                _window.addDockWidget(::Qt::DockWidgetArea::LeftDockWidgetArea, dock);
                _qtTileGrids.push_back(std::unique_ptr<QtTileGrid>(tileGrid));
                if (!firstDock) firstDock = dock;
                else _window.tabifyDockWidget(firstDock, dock);
            }
            _window.setWindowTitle("Multiple Tile Grids");
            CenterWindow();
            _window.show();
        }

    protected:
        UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->AddCircle(position, color, diameter);
            return nullptr;
        }
    };
}
