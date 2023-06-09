#pragma once

#include <Simp1e/UI/UITile.h>
#include <Simp1e/UI/UITileGrid.h>

#include <QDockWidget>
#include <QGuiApplication>
#include <QMainWindow>
#include <QScreen>
#include <unordered_map>

#include "QtTileGrid.h"

namespace Simp1e::UI::Qt {

    class QtMutlipleTileGrids : public UITileGrid {
        UITileGrid::Config                                          _config;
        std::vector<QtTileGrid*>                                    _qtTileGrids;
        std::unordered_map<UITileGrid::RenderingStyle, QtTileGrid*> _qtTileGridsByRenderingStyle;
        std::unordered_set<UITileGridElement*>                      _elements;

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
                _qtTileGridsByRenderingStyle[style] = tileGrid;
                if (!firstDock) firstDock = dock;
                else _window->tabifyDockWidget(firstDock, dock);
            }
            CenterWindow();
            _window->show();
        }

        RenderingStyle GetRenderingStyle() override { return RenderingStyle::Multiple; }

        UITileGrid* GetGridForRenderingStyle(UITileGrid::RenderingStyle renderingStyle) override {
            auto found = _qtTileGridsByRenderingStyle.find(renderingStyle);
            if (found == _qtTileGridsByRenderingStyle.end()) return nullptr;
            return found->second;
        }

        UITile* GetTile(const Maps::TilePosition& position) override {
            // This won't work with multi!
            qDebug() << "GetTile not implemented for QtMutlipleTileGrids";
            return nullptr;
        }

        Maps::TileGrid* GetGrid(uint32_t layer = 0) override {
            // This won't work with multi!
            qDebug() << "GetGrid not implemented for QtMutlipleTileGrids";
            return nullptr;
        }

        std::vector<Maps::TilePosition> GetPath(
            const Maps::TilePosition& startPosition, const Maps::TilePosition& endPosition,
            bool hexgrid = false, bool allowDiagonalMovement = true
        ) override {
            return {};
        }

        bool SetTileObstacle(const Maps::TilePosition& position, bool obstacle) override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->SetTileObstacle(position, obstacle);
            return true;
        }

        bool SetResizeModeEnabled(UITileGridElement* element, bool enabled = true) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i)
                    _qtTileGrids[i]->SetResizeModeEnabled(elements.at(i), enabled);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }
        bool SetMoveModeEnabled(UITileGridElement* element, bool enabled = true) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i)
                    _qtTileGrids[i]->SetMoveModeEnabled(elements.at(i), enabled);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }
        bool SetRotateModeEnabled(UITileGridElement* element, bool enabled = true) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i)
                    _qtTileGrids[i]->SetRotateModeEnabled(elements.at(i), enabled);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }

        // TODO refactor the bit about getting the element into some helper plz
        bool SetBorder(
            UITileGridElement* element, bool enabled, UIColor color = {},
            UILineStyle style = UILineStyle::Solid
        ) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i)
                    _qtTileGrids[i]->SetBorder(elements.at(i), enabled, color, style);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }

        bool OnLeftClick(std::function<void(const Maps::TilePosition&)> handler, uint32_t layer)
            override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->OnLeftClick(handler, layer);
            return true;
        }
        bool OnRightClick(std::function<void(const Maps::TilePosition&)> handler, uint32_t layer)
            override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->OnRightClick(handler, layer);
            return true;
        }
        bool OnMiddleClick(std::function<void(const Maps::TilePosition&)> handler, uint32_t layer)
            override {
            for (auto& tileGrid : _qtTileGrids) tileGrid->OnMiddleClick(handler, layer);
            return true;
        }

        bool MoveElement(UITileGridElement* element, const Maps::TilePosition& position) override {
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

        bool AnimatedMoveElement(
            UITileGridElement* element, const Maps::TilePosition& position, double duration = 500,
            double delay = 0
        ) override {
            try {
                auto elements =
                    std::any_cast<std::vector<UITileGridElement*>>(element->GetElement());
                for (size_t i = 0; i < _qtTileGrids.size(); ++i) {
                    qDebug(
                    ) << "AnimatedMoveElement for grid: "
                      << GetDockNameForRenderingStyle(_qtTileGrids[i]->GetRenderingStyle()).c_str();
                    _qtTileGrids[i]->AnimatedMoveElement(elements.at(i), position, duration, delay);
                }
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
                _elements.erase(element);
                return true;
            } catch (const std::bad_any_cast&) {
                return false;
            }
        }

        UITileGridElement* AddCircle(
            const Maps::TilePosition& position, const UIColor& color, uint32_t diameter
        ) override {
            std::vector<UITileGridElement*> elements;
            for (auto& tileGrid : _qtTileGrids)
                elements.push_back(tileGrid->AddCircle(position, color, diameter));
            auto* element = new UITileGridElement(position, elements);
            _elements.insert(element);
            return element;
        }

        UITileGridElement* AddImage(
            const Maps::TilePosition& position, const std::filesystem::path& imagePath,
            bool angleTile = false
        ) override {
            std::vector<UITileGridElement*> elements;
            for (auto& tileGrid : _qtTileGrids)
                elements.push_back(tileGrid->AddImage(position, imagePath, angleTile));
            auto* element = new UITileGridElement(position, elements);
            _elements.insert(element);
            return element;
        }
    };
}
