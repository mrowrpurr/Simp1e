#pragma once

#include <Simp1e/Maps/TileGrid.h>

#include <algorithm>
#include <cmath>
#include <utility>
#include <vector>

#include "AStarTile.h"

namespace Simp1e::Maps::AStar {

    namespace Utility {

        std::vector<std::pair<AStarTile*, double>> GetHexNeighbours(
            std::vector<std::vector<AStarTile>>& grid, AStarTile& tile,
            bool diagonalMovementAllowed = true
        ) {
            std::vector<std::pair<AStarTile*, double>> neighbours;

            // list of the six direction vectors for a hex grid
            // with corresponding cost multipliers
            std::vector<std::pair<std::pair<int, int>, double>> directions = {
                {std::make_pair(-1, 0),  1.5}, // west
                {std::make_pair(1,  0),  1.5}, // east
                {std::make_pair(0,  -1), 1.0}, // northwest if x is even, northeast if x is odd
                {std::make_pair(0,  1),  1.0}, // southwest if x is even, southeast if x is odd
                {std::make_pair(-1, -1), 1.0}, // northwest if x is odd, northeast if x is even
                {std::make_pair(1,  1),  1.0}  // southwest if x is odd, southeast if x is even
            };

            for (auto& dir : directions) {
                int dx = dir.first.first;
                int dy = dir.first.second;

                if (tile.x() % 2 == 0) {
                    if (dx == -1 && dy == -1)  // northwest if x is odd
                        continue;
                    if (dx == 1 && dy == 1)  // southwest if x is odd
                        continue;
                } else {
                    if (dx == 0 && dy == -1)  // northwest if x is even
                        continue;
                    if (dx == 0 && dy == 1)  // southwest if x is even
                        continue;
                }

                uint32_t checkX         = tile.x() + dx;
                uint32_t checkY         = tile.y() + dy;
                double   costMultiplier = dir.second;

                // check if the tile coordinates are within the map
                if (checkX >= 0 && checkX < grid.size() && checkY >= 0 && checkY < grid[0].size()) {
                    auto thisTile = &grid[checkX][checkY];

                    // Other checks
                    if (thisTile->IsObstacle()) continue;
                    if (thisTile->z() != tile.z()) continue;

                    // Howdy, neighbour!
                    neighbours.push_back({thisTile, costMultiplier});
                }
            }

            return neighbours;
        }

        std::vector<std::pair<AStarTile*, double>> GetRectangleNeighbours(
            std::vector<std::vector<AStarTile>>& grid, AStarTile& tile, bool diagonalMovementAllowed
        ) {
            std::vector<std::pair<AStarTile*, double>> neighbours;

            // check the tiles around the current tile
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0)  // skip the current tile
                        continue;

                    uint32_t checkX = tile.x() + x;
                    uint32_t checkY = tile.y() + y;
                    // check if the tile coordinates are within the map
                    if (checkX >= 0 && checkX < grid.size() && checkY >= 0 &&
                        checkY < grid[0].size()) {
                        // Other checks
                        auto* thisTile = &grid[checkX][checkY];
                        if (thisTile->IsObstacle()) continue;  // skip obstacles (walls, etc.)
                        if (thisTile->z() != tile.z())
                            continue;  // skip tiles on other layers (floors, etc. )

                        // Howdy, neighbour!
                        double costModifier = 1.0;
                        if (diagonalMovementAllowed && std::abs(x) == std::abs(y)) {
                            costModifier = 0.75;  // Example cost for diagonal movement
                        }
                        neighbours.push_back(std::make_pair(thisTile, costModifier));
                    }
                }
            }

            return neighbours;
        }
    }

    std::vector<AStarTile> GetShortestPath(
        TileGrid& gameTileGrid, Tile* start, Tile* end, bool hextiles, bool diagonalMovementAllowed
    ) {
        // Initialize A* tile grid
        std::vector<std::vector<AStarTile>> grid;

        // Loop throughthe TileGrid rows and columns, adding new AStarTiles to the grid
        for (uint32_t row = 0; row < gameTileGrid.GetRows(); row++) {
            std::vector<AStarTile> gridRow;
            for (uint32_t col = 0; col < gameTileGrid.GetColumns(); col++)
                gridRow.push_back(AStarTile{gameTileGrid.GetTile(row, col)});
            grid.push_back(gridRow);
        }

        // Get the startTile and endTile AStarTile pointers from the grid
        auto& startTile = grid[start->GetPosition().x][start->GetPosition().y];
        auto& endTile   = grid[end->GetPosition().x][end->GetPosition().y];

        // initialize the open list with the starting tile
        std::vector<AStarTile*> openList = {&startTile};

        // initialize the closed list
        std::vector<AStarTile*> closedList;

        while (!openList.empty()) {
            // sort the open list to find the tile with the lowest fCost and hCost
            std::sort(openList.begin(), openList.end(), [](AStarTile* a, AStarTile* b) {
                return a->GetFCost() < b->GetFCost() ||
                       (a->GetFCost() == b->GetFCost() && a->hCost < b->hCost);
            });

            AStarTile* currentTile = openList.front();
            openList.erase(openList.begin());

            // Move currentTile from open list to closed list
            closedList.push_back(currentTile);

            // check the neighbours of the current tile
            auto neighbours =
                (hextiles)
                    ? Utility::GetHexNeighbours(grid, *currentTile, diagonalMovementAllowed)
                    : Utility::GetRectangleNeighbours(grid, *currentTile, diagonalMovementAllowed);

            for (auto& neighbourPair : neighbours) {
                AStarTile* neighbour      = neighbourPair.first;
                double     costMultiplier = neighbourPair.second;

                // Apply cost multiplier to gCost
                float gCost = currentTile->gCost + costMultiplier;
                // float gCost = currentTile->gCost + ((neighbour->x() - currentTile->x() != 0 &&
                //                                      neighbour->y() - currentTile->y() != 0)
                //                                         ? costMultiplier
                //                                         : 0.6f * costMultiplier);

                // Skip neighbour if it is in the closed list
                if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
                    continue;
                if (neighbour->IsObstacle()) continue;

                // Assumes all movements are 1 unit
                float dx = std::abs(static_cast<int>(neighbour->x() - endTile.x()));
                float dy = std::abs(static_cast<int>(neighbour->y() - endTile.y()));
                float hCost;

                // Modify heuristic calculation for hex grid
                if ((endTile.x() % 2 == 0 && endTile.y() % 2 == 0) ||
                    (endTile.x() % 2 != 0 && endTile.y() % 2 != 0)) {
                    hCost = dx + dy;
                } else {
                    hCost = dx + dy + 0.5;
                }

                // if the new path to the neighbour is shorter or if the neighbour has not been
                // processed
                if (!neighbour->IsObstacle() && !neighbour->parent || gCost < neighbour->gCost) {
                    neighbour->gCost  = gCost;
                    neighbour->hCost  = hCost;
                    neighbour->parent = currentTile;

                    if (neighbour == &endTile) {
                        // construct the shortest path
                        std::vector<AStarTile> path;
                        AStarTile*             tile = &endTile;
                        while (tile) {
                            path.push_back(*tile);
                            tile = tile->parent;
                        }
                        // reverse the path to start from the beginning
                        std::reverse(path.begin(), path.end());
                        return path;
                    }

                    openList.push_back(neighbour);
                }
            }
        }

        // no path could be found
        return {};
    }

    // float dx    = std::abs(static_cast<int>(neighbour->x() - endTile.x()));
    // float dy    = std::abs(static_cast<int>(neighbour->y() - endTile.y()));
    // float hCost = std::max(dx, dy);

    // for (AStarTile* neighbour : neighbours) {
    //     // In hex grid, every move is equivalent, hence gCost should always
    //     increment by 1. float gCost = currentTile->gCost + 1.0f;

    //     // if (!hextiles) {
    //     //     float gCost = currentTile->gCost + ((neighbour->x() -
    //     currentTile->x()
    //     != 0
    //     //     &&
    //     //                                          neighbour->y() -
    //     currentTile->y()
    //     != 0)
    //     //                                             ? 1
    //     //                                             : 0.6f);
    //     // }
}