#pragma once

#include <cstdint>

#include "../Tile.h"

namespace Prototyping::UI {

    class UITileGridElement {
        Tile::Position _position;
        uint32_t       _width;
        uint32_t       _height;

    public:
        UITileGridElement(const Tile::Position& position, uint32_t width, uint32_t height)
            : _position(position), _width(width), _height(height) {}

        const Tile::Position& GetPosition() const { return _position; }
        uint32_t              GetWidth() const { return _width; }
        uint32_t              GetHeight() const { return _height; }
    };
}