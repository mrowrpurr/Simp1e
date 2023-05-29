#pragma once

#include <algorithm>
#include <cmath>
#include <vector>

#include "../TileGrid.h"
#include "AStarTile.h"

namespace Prototyping::AStar {

    namespace Utility {

        std::vector<AStarTile*> GetNeighbours(
            std::vector<std::vector<AStarTile>>& grid, AStarTile& tile,
            bool diagonalMovementAllowed = true
        ) {
            std::vector<AStarTile*> neighbours;

            // list of the six direction vectors for a pointy-top hex grid
            std::vector<std::pair<int, int>> even_directions = {
                {-1, 0 }, // northeast
                {0,  1 }, // east
                {1,  0 }, // southeast
                {1,  -1}, // southwest
                {0,  -1}, // west
                {-1, -1}  // northwest
            };

            std::vector<std::pair<int, int>> odd_directions = {
                {-1, 1 }, // northeast
                {0,  1 }, // east
                {1,  1 }, // southeast
                {1,  0 }, // southwest
                {0,  -1}, // west
                {-1, 0 }  // northwest
            };

            auto& directions = (tile.y() % 2 == 0) ? even_directions : odd_directions;

            for (auto& dir : directions) {
                uint32_t checkX = tile.x() + dir.first;
                uint32_t checkY = tile.y() + dir.second;

                // check if the tile coordinates are within the map
                if (checkX >= 0 && checkX < grid.size() && checkY >= 0 && checkY < grid[0].size()) {
                    auto thisTile = &grid[checkX][checkY];

                    // Other checks
                    if (thisTile->IsObstacle()) continue;
                    if (thisTile->z() != tile.z()) continue;

                    // Howdy, neighbour!
                    neighbours.push_back(thisTile);
                }
            }

            return neighbours;
        }

        // TODO update to only pay attention to other tiles on the same Z layer
        std::vector<AStarTile*> GetRectangleNeighbours(
            std::vector<std::vector<AStarTile>>& grid, AStarTile& tile, bool diagonalMovementAllowed
        ) {
            // return GetIsometricNeighbours(grid, tile, diagonalMovementAllowed);

            std::vector<AStarTile*> neighbours;

            // check the tiles around the current tile
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0)  // skip the current tile
                        continue;
                    if (!diagonalMovementAllowed &&
                        std::abs(x) == std::abs(y))  // skip diagonal tiles
                        continue;

                    uint32_t checkX = tile.x() + x;
                    uint32_t checkY = tile.y() + y;
                    // check if the tile coordinates are within the map
                    if (checkX >= 0 && checkX < grid.size() && checkY >= 0 &&
                        checkY < grid[0].size()) {
                        // Other checks
                        auto* thisTile = &grid[checkX][checkY];
                        if (thisTile->IsObstacle()) continue;  // skip obstacles (walls, etc.
                        if (thisTile->z() != tile.z())
                            continue;  // skip tiles on other layers (floors, etc. )

                        // Howdy, neighbour!
                        neighbours.push_back(thisTile);
                    }
                }
            }

            return neighbours;
        }
    }

    std::vector<AStarTile> GetShortestPath(
        TileGrid& gameTileGrid, Tile* start, Tile* end, bool diagonalMovementAllowed
    ) {
        // Initialize A* tile grid
        std::vector<std::vector<AStarTile>> grid;

        // Loop throughthe TileGrid rows and columns, adding new AStarTiles to the grid vector
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
            for (AStarTile* neighbour :
                 Utility::GetNeighbours(grid, *currentTile, diagonalMovementAllowed)) {
                // More expensive diagonal movement
                // float gCost = currentTile->gCost + std::hypot(
                //                                        neighbour->x() - currentTile->x(),
                //                                        neighbour->y() - currentTile->y()
                //                                    );

                // Less expensive diagonal movement
                float gCost = currentTile->gCost + ((neighbour->x() - currentTile->x() != 0 &&
                                                     neighbour->y() - currentTile->y() != 0)
                                                        ? 1
                                                        : 1);

                // Skip neighbour if it is in the closed list
                if (std::find(closedList.begin(), closedList.end(), neighbour) != closedList.end())
                    continue;
                if (neighbour->IsObstacle()) continue;

                float hCost =
                    std::hypot(neighbour->x() - endTile.x(), neighbour->y() - endTile.y());

                // if the new path to the neighbour is shorter or if the neighbour has not been
                // processed
                if (!neighbour->parent || gCost < neighbour->gCost) {
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
}