#pragma once

#include <Simp1e/Maps/Tile.h>
#include <Simp1e/UI/UILineStyle.h>
#include <QWidget>
#include <filesystem>
#include <functional>

#include <Simp1e/Maps/TileGrid.h>

#include <Simp1e/UI/UIColor.h>
#include <Simp1e/UI/UIImage.h>
#include "UITile.h"
#include "UITileGridElement.h"

namespace Simp1e::UI {
    struct UITileGrid {
        enum class RenderingStyle {
            None,
            Grid,
            Isometric,
            Hexagons,
            IsometricWithHexagons,
            Multiple,
            Other,
        };

        struct Config {
            Maps::TileGrid* grid               = nullptr;
            RenderingStyle  renderingStyle     = RenderingStyle::Grid;
            uint32_t        tileWidth          = 128;
            uint32_t        tileHeight         = 96;
            uint32_t        padding            = 0;
            bool            showGrid           = true;
            bool            displayCoordinates = true;
        };

        virtual ~UITileGrid() = default;
        virtual RenderingStyle                  GetRenderingStyle() { return RenderingStyle::None; }
        virtual bool                            ShowAsWindow() { return false; }
        virtual QWidget*                        GetWidget() { return nullptr; }
        virtual Maps::TileGrid*                 GetGrid(uint32_t layer = 0) { return nullptr; }
        virtual std::vector<Maps::TilePosition> GetPath(
            const Maps::TilePosition& startPosition, const Maps::TilePosition& endPosition,
            bool hexgrid = false, bool allowDiagonalMovement = true
        ) {
            return {};
        }

        // TODO remove if we don't need it, it's use for special support with the multigrid
        virtual UITileGrid* GetGridForRenderingStyle(RenderingStyle renderingStyle) {
            return nullptr;
        }

        virtual UITile* GetTile(const Maps::TilePosition& position) { return nullptr; }
        virtual bool SetTileObstacle(const Maps::TilePosition& position, bool isObstacle = true) {
            return false;
        }
        virtual bool SetBackgroundColor(const UIColor& color) { return false; }
        virtual bool AddBackgroundImage(const UIImage& image) { return false; }
        virtual bool RemoveBackgroundImage(const UIImage& image) { return false; }
        virtual bool MoveElement(UITileGridElement* element, const Maps::TilePosition& position) {
            return false;
        }
        virtual bool AnimatedMoveElement(
            UITileGridElement* element, const Maps::TilePosition& position, double duration = 500,
            double delay = 0
        ) {
            return false;
        }
        virtual bool RemoveElement(UITileGridElement* element) { return false; }
        virtual bool SetResizeModeEnabled(UITileGridElement* element, bool enabled = true) {
            return false;
        }
        virtual bool SetMoveModeEnabled(UITileGridElement* element, bool enabled = true) {
            return false;
        }
        virtual bool SetRotateModeEnabled(UITileGridElement* element, bool enabled = true) {
            return false;
        }
        virtual bool SetBorder(
            UITileGridElement* element, bool enabled, UIColor color = {},
            UILineStyle style = UILineStyle::Solid
        ) {
            return false;
        }
        virtual UITileGridElement* AddCircle(
            const Maps::TilePosition& position, const UIColor& color, uint32_t diameter
        ) {
            return nullptr;
        }
        virtual UITileGridElement* AddRectangle(
            const Maps::TilePosition& position, const UIColor& color, uint32_t width,
            uint32_t height
        ) {
            return nullptr;
        }
        virtual UITileGridElement* AddImage(
            const Maps::TilePosition& position, const std::filesystem::path& imagePath,
            bool angleTile = false
        ) {
            return nullptr;
        }
        virtual bool OnLeftClick(
            std::function<void(const Maps::TilePosition&)>, uint32_t layer = 0
        ) {
            return false;
        }
        virtual bool OnRightClick(
            std::function<void(const Maps::TilePosition&)>, uint32_t layer = 0
        ) {
            return false;
        }
        virtual bool OnMiddleClick(
            std::function<void(const Maps::TilePosition&)>, uint32_t layer = 0
        ) {
            return false;
        }
    };
}