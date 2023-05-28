#pragma once

#include <QWidget>
#include <functional>

#include "../Tile.h"
#include "../TileGrid.h"
#include "UIColor.h"
#include "UIImage.h"
#include "UITile.h"
#include "UITileGridElement.h"

namespace Prototyping::UI {
    struct UITileGrid {
        enum class RenderingStyle { Grid, Isometric, Hexagons, IsometricWithHexagons };

        struct Config {
            TileGrid*      grid               = nullptr;
            RenderingStyle renderingStyle     = RenderingStyle::Grid;
            uint32_t       tileWidth          = 64;
            uint32_t       tileHeight         = 48;
            uint32_t       padding            = 4;
            bool           showGrid           = true;
            bool           displayCoordinates = true;
        };

        virtual ~UITileGrid() = default;
        virtual bool               ShowAsWindow() { return false; }
        virtual QWidget*           GetWidget() { return nullptr; }
        virtual UITile*            GetTile(const Tile::Position& position) { return nullptr; }
        virtual bool               SetBackgroundColor(const UIColor& color) { return false; }
        virtual bool               AddBackgroundImage(const UIImage& image) { return false; }
        virtual bool               RemoveBackgroundImage(const UIImage& image) { return false; }
        virtual UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) {
            return nullptr;
        }
        virtual UITileGridElement* AddRectangle(
            const Tile::Position& position, const UIColor& color, uint32_t width, uint32_t height
        ) {
            return nullptr;
        }
        virtual bool OnLeftClick(std::function<void(const Tile::Position&)>) { return false; }
        virtual bool OnRightClick(std::function<void(const Tile::Position&)>) { return false; }
        virtual bool OnMiddleClick(std::function<void(const Tile::Position&)>) { return false; }
    };
}