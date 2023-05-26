#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>

enum CellType { FREE, OBSTACLE, START, END };

class Cell;

struct IGame {
    virtual void SetStartCell(Cell* cell) = 0;
    virtual void SetEndCell(Cell* cell)   = 0;
};

class Cell : public QGraphicsRectItem {
    QBrush defaultBrush;
    QBrush obstacleBrush;
    QBrush startBrush;
    QBrush endBrush;
    IGame* game;

public:
    Cell(IGame* g, qreal x, qreal y, qreal w, qreal h, QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(x, y, w, h, parent),
          defaultBrush(Qt::white),
          obstacleBrush(Qt::black),
          startBrush(Qt::green),
          endBrush(Qt::red),
          game(g) {
        setBrush(defaultBrush);
    }

    // A* shit
    CellType           type    = FREE;
    bool               visited = false;
    float              globalGoal;        // also known as "g"
    float              localGoal;         // also known as "f"
    std::vector<Cell*> vecNeighbors;      // cells this cell is connected to
    Cell*              parent = nullptr;  // cell connecting to this cell

    void setType(CellType newType) {
        switch (newType) {
            case OBSTACLE:
                setBrush(obstacleBrush);
                break;
            case START:
                setBrush(startBrush);
                break;
            case END:
                setBrush(endBrush);
                break;
            default:
                setBrush(defaultBrush);
                break;
        }
    }

    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            game->SetStartCell(this);
            setType(START);
        } else if (event->button() == Qt::MiddleButton) {
            game->SetEndCell(this);
            setType(END);
        } else if (event->button() == Qt::RightButton) {
            setType(OBSTACLE);
        }
    }
};

class Game : public IGame {
    QGraphicsScene*                 scene;
    std::vector<std::vector<Cell*>> grid;
    Cell*                           startCell = nullptr;
    Cell*                           endCell   = nullptr;
    int                             cellSize  = 50;

public:
    Game() {
        scene = new QGraphicsScene;
        scene->setSceneRect(0, 0, 500, 500);
        scene->setBackgroundBrush(Qt::black);
    }

    QGraphicsScene* getScene() const { return scene; }

    void SetStartCell(Cell* cell) override {
        if (startCell) startCell->setType(FREE);
        startCell = cell;
    }
    void SetEndCell(Cell* cell) override {
        if (endCell) endCell->setType(FREE);
        endCell = cell;
    }

    void createGrid(int rows, int cols) {
        grid.resize(rows, std::vector<Cell*>(cols));

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                auto* cell = new Cell(this, j * cellSize, i * cellSize, cellSize, cellSize);
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
        if (endCell->parent == nullptr) {
            return {};  // No path.
        }

        std::vector<Cell*> path;
        Cell*              current = endCell;
        while (current != startCell) {
            path.push_back(current);
            current = current->parent;
        }

        std::reverse(path.begin(), path.end());  // We want the path to go from start to end.

        return path;
    }

    void Solve() {
        if (startCell && endCell) {
            solveAStar(startCell, endCell);
            auto path = getShortestPath(startCell, endCell);
            qDebug() << "Path size: " << path.size();
            for (const auto& cell : path) {
                qDebug() << "(" << cell->x() / cellSize << ", " << cell->y() / cellSize << ")";
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