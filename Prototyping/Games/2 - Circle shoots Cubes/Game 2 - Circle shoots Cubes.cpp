#include <QApplication>
#include <QColor>
#include <QDebug>
#include <QGraphicsEllipseItem>
#include <QGraphicsObject>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QKeyEvent>
#include <QPainter>
#include <QPropertyAnimation>
#include <QTimer>
#include <functional>
#include <map>
#include <random>

struct RGBColor {
    int red, green, blue;
};

class ClickableRectItem : public QGraphicsRectItem {
    std::function<void()> _onClickHandler = []() {};

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

class AnimatedEllipse : public QGraphicsObject {
public:
    AnimatedEllipse(qreal x, qreal y, qreal w, qreal h) : rect(x, y, w, h) { setPos(x, y); }

    QRectF boundingRect() const override { return rect; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
        override {
        painter->setBrush(Qt::blue);
        painter->drawEllipse(rect);
    }

private:
    QRectF rect;
};

class GridMap {
    std::pair<int, int>              _currentCirclePosition;
    std::vector<std::pair<int, int>> _currentCubePositions;

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

    void AddCube(int row, int col) {
        _currentCubePositions.push_back({row, col});
        Redraw();
    }

    void MoveCircleTo(int row, int col) {
        _currentCirclePosition = {row, col};
        // single shot timer to avoid flickering
        QTimer::singleShot(0, [this, row, col]() {
            Redraw();
            // emit CircleMoved(row, col);
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

    void Redraw() {
        scene->clear();
        drawGrid();
        drawCubes();
        drawCircleOnTile(
            _currentCirclePosition.first, _currentCirclePosition.second, {207, 2, 152}
        );
    }

private:
    std::map<std::pair<int, int>, QGraphicsRectItem*>& getCubeItems() { return _currentCubeItems; }

    void fireProjectile(std::pair<int, int> from, std::pair<int, int> to) {
        qDebug() << "Firing projectile from" << from.first << "," << from.second << "to" << to.first
                 << "," << to.second;

        auto startingPoint = std::make_pair(
            from.second * cellHeight + cellHeight / 2, from.first * cellWidth + cellWidth / 2
        );
        auto endingPoint = std::make_pair(
            to.first * cellHeight + cellHeight / 2, to.second * cellWidth + cellWidth / 2
        );

        // Create a small ellipse representing the projectile
        AnimatedEllipse* projectile = new AnimatedEllipse(0, 0, 10, 10);

        // Add the projectile to the scene
        scene->addItem(projectile);

        QObject* projectilePtr = static_cast<QObject*>(projectile);

        // Create a property animation for the x and y coordinates
        QPropertyAnimation* animation = new QPropertyAnimation(projectile, "pos");
        animation->setDuration(1000);  // Animation duration in milliseconds
        animation->setStartValue(QPointF(startingPoint.first, startingPoint.second));
        animation->setEndValue(QPointF(endingPoint.second, endingPoint.first));

        // Connect the finished signal of the animations to a lambda function that removes and
        // deletes the projectile
        QObject::connect(animation, &QPropertyAnimation::finished, [projectile, this, to]() {
            scene->removeItem(projectile);
            delete projectile;
            qDebug() << "Projectile reached" << to.first << "," << to.second;
            auto foundCube = getCubeItems().find(to);
            if (foundCube != _currentCubeItems.end()) {
                qDebug() << "Found cube to explode";
                explode(foundCube->second);
                _currentCubeItems.erase(foundCube);
            }
        });

        // Start the animations
        animation->start(QAbstractAnimation::DeleteWhenStopped);
    }

    std::pair<int, int> getCurrentCirclePosition() { return _currentCirclePosition; }

    void explode(QGraphicsRectItem* cube) {
        if (cube->x() == 0) {
            qDebug() << "Something is wrong, the cube is at 0,0";
        }

        auto cubePos = cube->pos();
        auto cubeX   = cubePos.x();
        auto cubeY   = cubePos.y();

        std::default_random_engine            generator;
        std::uniform_real_distribution<qreal> distribution(-50.0, 50.0);

        qDebug() << "Explode cube which is at " << cubeX << "," << cubeY;

        // Create a bunch of particles at the cube's location
        const int numParticles = 100;
        for (int i = 0; i < numParticles; ++i) {
            AnimatedEllipse* particle =
                new AnimatedEllipse(cubeX, cubeY, 5, 5);  // Adjust size as necessary
            scene->addItem(particle);

            // Create a property animation for the x and y coordinates
            QPropertyAnimation* animation = new QPropertyAnimation(particle, "pos");
            animation->setDuration(500);  // Animation duration in milliseconds

            // Set the end position to a random offset from the cube's position
            qreal endX = cubeX + distribution(generator);
            qreal endY = cubeY + distribution(generator);
            animation->setEndValue(QPointF(endX, endY));

            // Connect the finished signal of the animations to a lambda function that removes and
            // deletes the particle
            QObject::connect(animation, &QPropertyAnimation::finished, [particle, this]() {
                scene->removeItem(particle);
                delete particle;
            });

            // Start the animation
            animation->start(QAbstractAnimation::DeleteWhenStopped);
        }

        // Hide or delete the cube
        scene->removeItem(cube);
        delete cube;
    }

    void drawGrid() {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int offsetX = j * cellWidth;
                int offsetY = i * cellHeight;

                QRectF rect(offsetX, offsetY, cellWidth, cellHeight);

                ClickableRectItem* item = new ClickableRectItem(rect);
                item->OnClick([this, i, j]() {
                    qDebug() << "Clicked on tile" << i << j;
                    qDebug() << "Current circle position is" << getCurrentCirclePosition();
                    auto from = getCurrentCirclePosition();
                    fireProjectile(from, {i, j});
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

    std::map<std::pair<int, int>, QGraphicsRectItem*> _currentCubeItems;

    void drawCubes() {
        _currentCubeItems.clear();
        for (auto& cube : _currentCubePositions) {
            int offsetX = cube.second * cellWidth;
            int offsetY = cube.first * cellHeight;

            int    size = cellWidth / 2;  // You can adjust these values as you need
            QRectF rect(offsetX + size / 2, offsetY + size / 2, size, size);

            QGraphicsRectItem* item = new QGraphicsRectItem(rect);
            qDebug() << "Adding current cube item at " << cube.first << "," << cube.second;
            _currentCubeItems[{cube.first, cube.second}] = item;
            item->setBrush(Qt::blue);
            scene->addItem(item);
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
    map.AddCube(1, 2);
    map.AddCube(8, 7);
    map.AddCube(3, 8);

    KeyPressGraphicsView view(map.getScene());
    view.OnUpKey([&map]() { map.MoveCircleUp(); });
    view.OnDownKey([&map]() { map.MoveCircleDown(); });
    view.OnLeftKey([&map]() { map.MoveCircleLeft(); });
    view.OnRightKey([&map]() { map.MoveCircleRight(); });
    view.show();

    return app.exec();
}
