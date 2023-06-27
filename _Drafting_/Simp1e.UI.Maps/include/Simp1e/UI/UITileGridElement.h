#pragma once

#include <Simp1e/Maps/Tile.h>
#include <Simp1e/Maps/TilePosition.h>

#include <any>
#include <cstdint>

namespace Simp1e::UI {

    class UITileGridElement {
        Maps::TilePosition _position;
        std::any           _element;

    public:
        UITileGridElement(const Maps::TilePosition& position, std::any element)
            : _position(position), _element(element) {}

        void SetPosition(const Maps::TilePosition& position) { _position = position; }
        const Maps::TilePosition& GetPosition() const { return _position; }
        std::any&                 GetElement() { return _element; }
    };
}