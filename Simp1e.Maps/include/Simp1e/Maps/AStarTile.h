#pragma once

#include <Simp1e/Maps/Tile.h>

namespace Simp1e::AStar {
    struct AStarTile {
        /*
            A* stuff
        */
        AStarTile* parent = nullptr;
        float      gCost  = 0.0f;  // cost of moving from the starting tile to this tile
        float      hCost  = 0.0f;  // heuristic cost of moving from this tile to the ending tile

        // Get total cost of moving from the starting tile to the ending tile through this tile
        float GetFCost() const { return gCost + hCost; }

        /*
            Easy accessors for relevant tile data
        */
        Tile*    tile;
        uint32_t x() const { return tile->GetPosition().x; }
        uint32_t y() const { return tile->GetPosition().y; }
        uint32_t z() const { return tile->GetPosition().z; }
        bool     IsObstacle() const { return tile->IsObstacle(); }

        AStarTile(Tile* tile) : tile(tile) {}
    };
}