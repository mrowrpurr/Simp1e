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

struct RGBColor {
    int red, green, blue;
};

#include <QGraphicsSceneMouseEvent>

class ClickableRectItem : public QGraphicsRectItem {
    std::function<void()> _onClickHandler;

public:
    ClickableRectItem(const QRectF& rect, QGraphicsItem* parent = nullptr)
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
    GridMap(int rows, int cols) : rows(rows), cols(cols), cellWidth(50), cellHeight(50) {
        scene      = new QGraphicsScene();
        tileColors = std::map<std::pair<int, int>, QColor>();
        drawGrid();
    }

    QGraphicsScene* getScene() const { return scene; }

    void SetTileLines(bool state) { drawTileLines = state; }

    void SetTileColor(int row, int col, RGBColor color) {
        QColor qcolor(color.red, color.green, color.blue);
        tileColors[{row, col}] = qcolor;
    }

    void MoveCircleTo(int row, int col) {
        _currentCirclePosition = {row, col};
        // single shot timer to avoid flickering
        QTimer::singleShot(0, [this, row, col]() {
            scene->clear();
            drawGrid();
            this->drawCircleOnTile(row, col, {207, 2, 152});
        });
    }

    void MoveCircleUp() {
        if (_currentCirclePosition.first > 0)
            MoveCircleTo(_currentCirclePosition.first - 1, _currentCirclePosition.second);
    }
    void MoveCircleDown() {
        if (_currentCirclePosition.first < rows - 1)
            MoveCircleTo(_currentCirclePosition.first + 1, _currentCirclePosition.second);
    }
    void MoveCircleLeft() {
        if (_currentCirclePosition.second > 0)
            MoveCircleTo(_currentCirclePosition.first, _currentCirclePosition.second - 1);
    }
    void MoveCircleRight() {
        if (_currentCirclePosition.second < cols - 1)
            MoveCircleTo(_currentCirclePosition.first, _currentCirclePosition.second + 1);
    }

private:
    void drawGrid() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int offsetX = j * cellWidth;
                int offsetY = i * cellHeight;

                QRectF rect(offsetX, offsetY, cellWidth, cellHeight);

                ClickableRectItem* item = new ClickableRectItem(rect);
                item->OnClick([this, i, j]() {
                    qDebug() << "Clicked on tile" << i << j;
                    this->MoveCircleTo(i, j);
                });
                if (drawTileLines) {
                    item->setPen(QPen(Qt::black, 1));
                }
                if (tileColors.find({i, j}) != tileColors.end()) {
                    item->setBrush(tileColors[{i, j}]);
                } else {
                    item->setBrush(Qt::white);
                }

                scene->addItem(item);
            }
        }
    }

    void drawCircleOnTile(int row, int col, RGBColor color) {
        QColor qcolor(color.red, color.green, color.blue);
        int    offsetX = col * cellWidth;
        int    offsetY = row * cellHeight;

        int circleWidth  = cellWidth / 2;  // You can adjust these values as you need
        int circleHeight = cellHeight / 2;

        // Position circle in the middle of the cell
        offsetX += cellWidth / 4;
        offsetY += cellHeight / 4;

        QGraphicsEllipseItem* item =
            new QGraphicsEllipseItem(offsetX, offsetY, circleWidth, circleHeight);
        item->setBrush(qcolor);

        scene->addItem(item);
    }

    int                                   rows;
    int                                   cols;
    int                                   cellWidth;
    int                                   cellHeight;
    QGraphicsScene*                       scene;
    bool                                  drawTileLines;
    std::map<std::pair<int, int>, QColor> tileColors;
};

int main(int argc, char* argv[]) {
    QApplication app{argc, argv};

    GridMap map(10, 15);
    map.SetTileLines(true);
    map.MoveCircleTo(5, 5);

    KeyPressGraphicsView view(map.getScene());
    view.OnUpKey([&map]() { map.MoveCircleUp(); });
    view.OnDownKey([&map]() { map.MoveCircleDown(); });
    view.OnLeftKey([&map]() { map.MoveCircleLeft(); });
    view.OnRightKey([&map]() { map.MoveCircleRight(); });
    view.show();

    return app.exec();
}
