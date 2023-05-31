#pragma once

#include <Simp1e/Maps/Tile.h>

#include <any>
#include <cstdint>

namespace Prototyping::UI {

    class UITileGridElement {
        Tile::Position _position;
        std::any       _element;

    public:
        UITileGridElement(const Tile::Position& position, std::any element)
            : _position(position), _element(element) {}

        void                  SetPosition(const Tile::Position& position) { _position = position; }
        const Tile::Position& GetPosition() const { return _position; }
        std::any&             GetElement() { return _element; }
    };
}