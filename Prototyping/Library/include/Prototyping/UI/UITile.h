#pragma once

#include <string>

#include "../Tile.h"
#include "UIColor.h"
#include "UIImage.h"

namespace Prototyping::UI {
    struct UITile {
        virtual ~UITile() = default;
        virtual Tile::Position GetPosition() const { return {}; }
        virtual Tile*          GetTile() { return nullptr; }
        virtual bool           SetText(const std::string& text) { return false; }
        virtual bool           SetBackgroundColor(const UIColor& color) { return false; }
        virtual bool           AddImage(const UIImage& image) { return false; }
        virtual bool           RemoveImage(const UIImage& image) { return false; }
    };
}
