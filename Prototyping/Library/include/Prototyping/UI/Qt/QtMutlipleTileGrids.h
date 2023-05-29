#pragma once

#include <QDockWidget>
#include <QGuiApplication>
#include <QMainWindow>
#include <QScreen>
#include <unordered_map>

#include "../UITileGrid.h"
#include "QtTileGrid.h"

namespace Prototyping::UI::Qt {

    class QtMutlipleTileGrids : public UITileGrid {
        UITileGrid::Config       _config;
        std::vector<QtTileGrid*> _qtTileGrids;

        QMainWindow* _window;

        std::string GetDockNameForRenderingStyle(UITileGrid::RenderingStyle renderingStyle) {
            switch (renderingStyle) {
                case UITileGrid::RenderingStyle::Grid:
                    return "Rectangles";
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
            _window->adjustSize();
            auto* screen   = QGuiApplication::primaryScreen();
            auto  geometry = screen->availableGeometry();
            int   x        = (geometry.width() - _window->width()) / 2;
            int   y        = (geometry.height() - _window->height()) / 2;
            _window->move(x, y);
        }

    public:
        QtMutlipleTileGrids(
            const Config& config, std::vector<UITileGrid::RenderingStyle> renderingStyles
        )
            : _config(config) {
            _window                = new QMainWindow();
            QDockWidget* firstDock = nullptr;
            for (auto style : renderingStyles) {
                Config gridConfig         = config;
                gridConfig.renderingStyle = style;
                auto* tileGrid            = new QtTileGrid(gridConfig);
                auto* dock = new QDockWidget(GetDockNameForRenderingStyle(style).c_str());
                dock->setWidget(tileGrid->GetWidget());
                _window->addDockWidget(::Qt::DockWidgetArea::LeftDockWidgetArea, dock);
                _qtTileGrids.push_back(tileGrid);
                if (!firstDock) firstDock = dock;
                else _window->tabifyDockWidget(firstDock, dock);
            }
            CenterWindow();
            _window->show();
        }

        bool OnLeftClick(std::function<void(const Tile::Position&)> handler, uint32_t layer)
            override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->OnLeftClick(handler, layer);
            return true;
        }
        bool OnRightClick(std::function<void(const Tile::Position&)> handler, uint32_t layer)
            override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->OnRightClick(handler, layer);
            return true;
        }
        bool OnMiddleClick(std::function<void(const Tile::Position&)> handler, uint32_t layer)
            override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->OnMiddleClick(handler, layer);
            return true;
        }

        bool MoveElement(UITileGridElement* element, const Tile::Position& position) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i)
                    _qtTileGrids[i]->MoveElement(elements.at(i), position);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }

        bool RemoveElement(UITileGridElement* element) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i)
                    _qtTileGrids[i]->RemoveElement(elements.at(i));
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }

        UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) override {
            std::vector<UITileGridElement*> elements;
            for (auto& tileGrid : _qtTileGrids)
                elements.push_back(tileGrid->AddCircle(position, color, diameter));
            auto* element = new UITileGridElement(position, elements);
            return element;
        }
    };
}
