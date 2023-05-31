#pragma once

#include <string>

#include <Simp1e/Maps/Tile.h>

#include <Simp1e/UI/UIColor.h>
#include <Simp1e/UI/UIImage.h>

namespace Simp1e::UI {
    struct UITile {
        virtual ~UITile() = default;
        virtual Maps::Tile::Position GetPosition() const { return {}; }
        virtual Maps::Tile*          GetTile() { return nullptr; }
        virtual bool                 SetText(const std::string& text) { return false; }
        virtual bool                 SetBackgroundColor(const UIColor& color) { return false; }
        virtual bool                 AddImage(const UIImage& image) { return false; }
        virtual bool                 RemoveImage(const UIImage& image) { return false; }
    };
}
