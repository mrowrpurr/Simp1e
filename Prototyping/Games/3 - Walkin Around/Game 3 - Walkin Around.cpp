#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTimer>
#include <functional>
#include <map>
#include <queue>
#include <vector>

constexpr auto BOARD =
    "      *   \n"
    " **   *   \n"
    " **       \n"
    "        * \n"
    "***   *   \n"
    "   *  *   \n";

struct Tile {
    bool obstacle = false;
    int  x, y;
};

struct Node {
    Tile* tile;
    int   g_cost;
    int   h_cost;
    Node* parent;

    int f_cost() const { return g_cost + h_cost; }
};

typedef std::vector<std::vector<Tile>> TileGrid;

TileGrid theGrid;

void InitializeGridFromString(const std::string& text) {
    // Get the number of rows and columns from the text.
    int rows = 0;
    int cols = 0;
    for (auto c : text) {
        if (c == '\n') {
            ++rows;
            continue;
        }
        if (rows == 0) {
            ++cols;
        }
    }

    // Resize the grid to the correct size.
    theGrid.resize(rows);
    for (auto& row : theGrid) {
        row.resize(cols);
    }

    // Fill in the grid with the tiles.
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            theGrid[row][col].x = col;
            theGrid[row][col].y = row;
        }
    }

    // // Given a string with multiple lines of empty spaces and asterisks,
    // // create a grid of tiles where the asterisks are obstacles.
    int row = 0;
    int col = 0;
    for (auto c : text) {
        if (c == '\n') {
            ++row;
            col = 0;
            continue;
        }
        if (c == '*') {
            theGrid[row][col].obstacle = true;
        }
        ++col;
    }
}

struct RGBColor {
    int red, green, blue;
};

#include <QGraphicsSceneMouseEvent>

class GameGraphicsRectItem : public QGraphicsRectItem {
    std::function<void()> _onClickHandler;

public:
    GameGraphicsRectItem(const QRectF& rect, QGraphicsItem* parent = nullptr)
        : QGraphicsRectItem(rect, parent) {}

    void OnClick(std::function<void()> handler) { _onClickHandler = handler; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
        if (event->button() == Qt::LeftButton) {
            _onClickHandler();
        }
        QGraphicsRectItem::mousePressEvent(event);
    }
};

class KeyPressGraphicsView : public QGraphicsView {
    std::function<void()> _upKeyHandler;
    std::function<void()> _downKeyHandler;
    std::function<void()> _leftKeyHandler;
    std::function<void()> _rightKeyHandler;

public:
    KeyPressGraphicsView(QGraphicsScene* scene, QWidget* parent = nullptr)
        : QGraphicsView(scene, parent) {}

    void OnUpKey(std::function<void()> handler) { _upKeyHandler = handler; }
    void OnDownKey(std::function<void()> handler) { _downKeyHandler = handler; }
    void OnLeftKey(std::function<void()> handler) { _leftKeyHandler = handler; }
    void OnRightKey(std::function<void()> handler) { _rightKeyHandler = handler; }

protected:
    void keyPressEvent(QKeyEvent* event) override {
        switch (event->key()) {
            case Qt::Key_Up:
            case Qt::Key_W:
                _upKeyHandler();
                break;
            case Qt::Key_Down:
            case Qt::Key_S:
                _downKeyHandler();
                break;
            case Qt::Key_Left:
            case Qt::Key_A:
                _leftKeyHandler();
                break;
            case Qt::Key_Right:
            case Qt::Key_D:
                _rightKeyHandler();
                break;
            default:
                QGraphicsView::keyPressEvent(event);
                break;
        }
    }
};

class GridMap {
    std::pair<int, int> _currentCirclePosition;

public:
    GridMap() : cellWidth(50), cellHeight(50) {
        scene      = new QGraphicsScene();
        tileColors = std::map<std::pair<int, int>, QColor>();
        initializeGrid();
        initializeCircle(3, 4, {255, 0, 255});
    }

    QGraphicsScene* getScene() const { return scene; }

    void SetTileColor(int row, int col, RGBColor color) {
        QColor qcolor(color.red, color.green, color.blue);
        tileColors[{row, col}] = qcolor;
    }

    void MoveCircleTo(int row, int col) {
        _currentCirclePosition = {row, col};
        // Move the circle to the new position.
        circle->setPos(
            col * cellWidth + cellWidth / 2 - circleSize / 2,
            row * cellHeight + cellHeight / 2 - circleSize / 2
        );
    }

    void MoveCircleUp() {
        if (_currentCirclePosition.first > 0 &&
            !theGrid[_currentCirclePosition.first - 1][_currentCirclePosition.second].obstacle)
            MoveCircleTo(_currentCirclePosition.first - 1, _currentCirclePosition.second);
    }
    void MoveCircleDown() {
        if (_currentCirclePosition.first < rows - 1 &&
            !theGrid[_currentCirclePosition.first + 1][_currentCirclePosition.second].obstacle)
            MoveCircleTo(_currentCirclePosition.first + 1, _currentCirclePosition.second);
    }
    void MoveCircleLeft() {
        if (_currentCirclePosition.second > 0 &&
            !theGrid[_currentCirclePosition.first][_currentCirclePosition.second - 1].obstacle)
            MoveCircleTo(_currentCirclePosition.first, _currentCirclePosition.second - 1);
    }
    void MoveCircleRight() {
        if (_currentCirclePosition.second < cols - 1 &&
            !theGrid[_currentCirclePosition.first][_currentCirclePosition.second + 1].obstacle)
            MoveCircleTo(_currentCirclePosition.first, _currentCirclePosition.second + 1);
    }

private:
    void initializeGrid() {
        rows = theGrid.size();
        cols = theGrid[0].size();

        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int  offsetX = j * cellWidth;
                int  offsetY = i * cellHeight;
                auto tile    = theGrid[i][j];

                QRectF rect(offsetX, offsetY, cellWidth, cellHeight);

                GameGraphicsRectItem* item = new GameGraphicsRectItem(rect);
                item->OnClick([this, i, j]() {
                    qDebug() << "Clicked on tile" << i << j;
                    this->MoveCircleTo(i, j);
                });
                if (drawTileLines) {
                    item->setPen(QPen(Qt::black, 1));
                }
                if (tileColors.find({i, j}) != tileColors.end()) {
                    item->setBrush(tileColors[{i, j}]);
                } else if (tile.obstacle) {
                    item->setBrush(Qt::black);
                } else {
                    item->setBrush(Qt::white);
                }

                scene->addItem(item);
            }
        }
    }

    void initializeCircle(int row, int col, RGBColor color) {
        QColor qcolor(color.red, color.green, color.blue);

        circle = new QGraphicsEllipseItem(0, 0, circleSize, circleSize);
        circle->setBrush(qcolor);

        scene->addItem(circle);
    }

    int                                   circleSize = 30;
    int                                   rows       = 0;
    int                                   cols       = 0;
    QGraphicsEllipseItem*                 circle;
    int                                   cellWidth;
    int                                   cellHeight;
    QGraphicsScene*                       scene;
    bool                                  drawTileLines = true;
    std::map<std::pair<int, int>, QColor> tileColors;
};

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    InitializeGridFromString(BOARD);

    GridMap map;
    map.MoveCircleTo(3, 4);

    KeyPressGraphicsView view{map.getScene()};
    view.OnUpKey([&map]() { map.MoveCircleUp(); });
    view.OnDownKey([&map]() { map.MoveCircleDown(); });
    view.OnLeftKey([&map]() { map.MoveCircleLeft(); });
    view.OnRightKey([&map]() { map.MoveCircleRight(); });
    view.show();

    return app.exec();
}
