#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <algorithm>
#include <cmath>
#include <vector>

struct Tile {
    uint32_t x;
    uint32_t y;
    bool     isObstacle;
    float    gCost;   // cost of moving from the starting tile to this tile
    float    hCost;   // heuristic cost of moving from this tile to the ending tile
    Tile*    parent;  // pointer to the parent tile

    float fCost() const { return gCost + hCost; }  // total cost
};

class TileUI : public QGraphicsRectItem {
public:
    TileUI(qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(x, y, w, h, parent), row(row), col(col) {
        setBrush(defaultBrush);
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        // if (event->button() == Qt::LeftButton) {
        //     game->SetStartCell(this);
        //     setType(START);
        // } else if (event->button() == Qt::MiddleButton) {
        //     game->SetEndCell(this);
        //     setType(END);
        // } else if (event->button() == Qt::RightButton) {
        //     setType(OBSTACLE);
        // }
    }
};

class Game {
    QGraphicsScene* scene;
    int             rows     = 10;
    int             cols     = 10;
    int             cellSize = 50;

public:
    Game() {
        scene = new QGraphicsScene;
        scene->setSceneRect(0, 0, 500, 500);
        scene->setBackgroundBrush(Qt::black);
    }

    QGraphicsScene* getScene() const { return scene; }

    void createGrid(int rows, int cols) {
        grid.resize(rows, std::vector<Cell*>(cols));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                auto* cell = new Cell(this, j, i, j * cellSize, i * cellSize, cellSize, cellSize);
                grid[i][j] = cell;
                cell->setPen(QPen(Qt::black));
                scene->addItem(cell);
            }
        }

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (j > 0) grid[i][j]->vecNeighbors.push_back(grid[i][j - 1]);         // Left
                if (j < cols - 1) grid[i][j]->vecNeighbors.push_back(grid[i][j + 1]);  // Right
                if (i > 0) grid[i][j]->vecNeighbors.push_back(grid[i - 1][j]);         // Above
                if (i < rows - 1) grid[i][j]->vecNeighbors.push_back(grid[i + 1][j]);  // Below
            }
        }
    }

    void solveAStar(Cell* startCell, Cell* endCell) {
        for (const auto& row : grid) {
            for (const auto& cell : row) {
                cell->visited    = false;
                cell->globalGoal = INFINITY;
                cell->localGoal  = INFINITY;
                cell->parent     = nullptr;
            }
        }

        auto distance = [](Cell* a, Cell* b) {
            return std::hypot(b->pos().x() - a->pos().x(), b->pos().y() - a->pos().y());
        };

        auto heuristic = [distance](Cell* a, Cell* b) { return distance(a, b); };

        // Initial setup
        Cell* current         = startCell;
        startCell->localGoal  = 0.0f;
        startCell->globalGoal = heuristic(startCell, endCell);

        std::list<Cell*> notTestedCells;
        notTestedCells.push_back(startCell);

        while (!notTestedCells.empty() && current != endCell) {
            notTestedCells.sort([](const Cell* lhs, const Cell* rhs) {
                return lhs->globalGoal < rhs->globalGoal;
            });

            // Remove cells which have been visited.
            while (!notTestedCells.empty() && notTestedCells.front()->visited) {
                notTestedCells.pop_front();
            }

            if (notTestedCells.empty()) {
                break;
            }

            current          = notTestedCells.front();
            current->visited = true;

            for (auto neighbor : current->vecNeighbors) {
                if (!neighbor->visited && neighbor->type != OBSTACLE) {
                    notTestedCells.push_back(neighbor);
                }

                float possiblyLowerGoal = current->localGoal + distance(current, neighbor);

                if (possiblyLowerGoal < neighbor->localGoal) {
                    neighbor->parent    = current;
                    neighbor->localGoal = possiblyLowerGoal;

                    // Update the neighbor's globalGoal because it's the heuristic that we use to
                    // sort the notTestedCells list.
                    neighbor->globalGoal = neighbor->localGoal + heuristic(neighbor, endCell);
                }
            }
        }
    }

    std::vector<Cell*> getShortestPath(Cell* startCell, Cell* endCell) {
        std::unordered_map<Cell*, Cell*>                         cameFrom;
        std::unordered_map<Cell*, float>                         gScore;
        std::unordered_map<Cell*, float>                         fScore;
        std::priority_queue<Cell*, std::vector<Cell*>, CellComp> openSet;

        for (auto& row : grid)
            for (auto cell : row) gScore[cell] = std::numeric_limits<float>::max();

        gScore[startCell] = 0.f;
        fScore[startCell] = heuristic(startCell, endCell);
        openSet.push(startCell);

        while (!openSet.empty()) {
            Cell* current = openSet.top();
            openSet.pop();

            if (current == endCell) {
                return reconstructPath(cameFrom, current);
            }

            for (Cell* neighbor : current->vecNeighbors) {
                if (neighbor->getType() == Cell::Type::OBSTACLE) {
                    continue;
                }

                float tentative_gScore = gScore[current] + distance(current, neighbor);
                if (tentative_gScore < gScore[neighbor]) {
                    cameFrom[neighbor] = current;
                    gScore[neighbor]   = tentative_gScore;
                    fScore[neighbor]   = gScore[neighbor] + heuristic(neighbor, endCell);

                    if (std::find_if(openSet.begin(), openSet.end(), [neighbor](Cell* n) {
                            return n == neighbor;
                        }) == openSet.end())
                        openSet.push(neighbor);
                }
            }
        }

        // Return empty path if there is no valid path
        return {};
    }

    void Solve() {
        if (startCell && endCell) {
            solveAStar(startCell, endCell);
            auto path = getShortestPath(startCell, endCell);
            qDebug() << "Path size: " << path.size();
            for (const auto& cell : path) {
                qDebug() << "(" << cell->getRow() << ", " << cell->getCol() << ")";
            }
        }
    }
};

class OurView : public QGraphicsView {
    Game* game;

public:
    OurView(Game* g) : game(g) {
        setSceneRect(0, 0, 500, 500);
        setBackgroundBrush(Qt::black);
    }

    void keyPressEvent(QKeyEvent* event) {
        if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return) {
            game->Solve();
        }
    }
};

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};
    Game         game;
    game.createGrid(10, 10);
    OurView view{&game};
    view.setScene(game.getScene());
    view.show();
    return app.exec();
}