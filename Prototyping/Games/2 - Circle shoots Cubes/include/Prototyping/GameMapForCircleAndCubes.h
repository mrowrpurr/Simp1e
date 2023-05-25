#pragma once

#include <QDebug>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QTimer>
#include <map>
#include <random>

#include "AnimatedEllipse.h"
#include "GameGraphicsRectItem.h"
#include "RBGColor.h"

namespace Prototyping {

    class GameMapForCircleAndCubes {
        std::pair<int, int>              _currentCirclePosition;
        std::vector<std::pair<int, int>> _currentCubePositions;
        bool                             _initialized = false;
        RGBColor                         _circleColor = {207, 2, 152};
        QGraphicsEllipseItem*            _currentCircleItem;

    public:
        GameMapForCircleAndCubes(int rows, int cols)
            : rows(rows), cols(cols), cellWidth(50), cellHeight(50) {
            scene      = new QGraphicsScene();
            tileColors = std::map<std::pair<int, int>, QColor>();
            drawGrid();
            drawCircleOnTile(0, 0, _circleColor);
        }

        unsigned int    GetWidth() const { return cols * cellWidth; }
        unsigned int    GetHeight() const { return rows * cellHeight; }
        QGraphicsScene* GetScene() const { return scene; }

        void SetTileLines(bool state) { drawTileLines = state; }

        void SetTileColor(int row, int col, RGBColor color) {
            QColor qcolor(color.red, color.green, color.blue);
            tileColors[{row, col}] = qcolor;
        }

        void AddCube(int row, int col) {
            _currentCubePositions.push_back({row, col});
            drawCubes();
        }

        void MoveCircleTo(int row, int col) { _currentCirclePosition = {row, col}; }

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
        void redrawTile(int row, int col) { auto foundCube = getCubeItems().find({row, col}); }

        std::pair<int, int> gridCoordinates(QPointF scenePos) {
            int gridX = static_cast<int>(scenePos.x()) / cellWidth;
            int gridY = static_cast<int>(scenePos.y()) / cellHeight;
            return {gridY, gridX};
        }

        std::map<std::pair<int, int>, QGraphicsRectItem*>& getCubeItems() {
            return _currentCubeItems;
        }

        void explodeUnderProjectileIfAnyCubes(QPointF pos) {
            auto foundCube = getCubeItems().find(gridCoordinates(pos));
            if (foundCube != _currentCubeItems.end()) {
                qDebug() << "Found cube to explode";
                // explode in a single shot
                // QTimer::singleShot(0, [this, foundCube]() { explode(foundCube->second); });
                explode(foundCube->second);
                _currentCubeItems.erase(foundCube);
                _currentCubePositions.erase(
                    std::remove(
                        _currentCubePositions.begin(), _currentCubePositions.end(),
                        gridCoordinates(pos)
                    ),
                    _currentCubePositions.end()
                );
            }
        }

        void fireProjectile(std::pair<int, int> from, std::pair<int, int> to) {
            qDebug() << "Firing projectile from" << from.first << "," << from.second << "to"
                     << to.first << "," << to.second;

            auto startingPoint = std::make_pair(
                from.second * cellHeight + cellHeight / 2, from.first * cellWidth + cellWidth / 2
            );
            auto endingPoint = std::make_pair(
                to.first * cellHeight + cellHeight / 2, to.second * cellWidth + cellWidth / 2
            );

            // Create a small ellipse representing the projectile
            AnimatedEllipse* projectile = new AnimatedEllipse(0, 0, 10, 10);

            QTimer* timer = new QTimer();

            QObject::connect(timer, &QTimer::timeout, [projectile, this]() {
                if (projectile) {
                    QPointF scenePos = projectile->pos();
                    explodeUnderProjectileIfAnyCubes(scenePos);
                }
            });

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
                explodeUnderProjectileIfAnyCubes(projectile->pos());
                scene->removeItem(projectile);
                delete projectile;
                qDebug() << "Projectile reached" << to.first << "," << to.second;
            });

            // Start the animations
            animation->start(QAbstractAnimation::DeleteWhenStopped);
            timer->start(50);

            QObject::connect(animation, &QPropertyAnimation::finished, [timer]() {
                timer->stop();
                delete timer;
            });
        }

        std::pair<int, int> getCurrentCirclePosition() { return _currentCirclePosition; }

        void explode(QGraphicsRectItem* cube) {
            if (!cube) {
                qDebug() << "Cube is null";
                return;
            }
            if (cube->x() == 0) {
                qDebug() << "Something is wrong, the cube is at 0,0";
            }

            std::default_random_engine            generator;
            std::uniform_real_distribution<qreal> distribution(-50.0, 50.0);

            qDebug() << "Explode cube which is at " << cube->x() << "," << cube->y();

            // Create a bunch of particles at the cube's location
            const int numParticles = 100;
            for (int i = 0; i < numParticles; ++i) {
                auto particle = new AnimatedEllipse(0, 0, 5, 5);
                particle->setPos(cube->scenePos());
                scene->addItem(particle);

                // Create a property animation for the x and y coordinates
                QPropertyAnimation* animation = new QPropertyAnimation(particle, "pos");
                animation->setDuration(500);  // Animation duration in milliseconds

                // Set the end position to a random offset from the cube's position
                qreal endX = cube->x() + distribution(generator);
                qreal endY = cube->y() + distribution(generator);
                animation->setEndValue(QPointF(endX, endY));

                // Connect the finished signal of the animations to a lambda function that removes
                // and deletes the particle
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

                    GameGraphicsRectItem* item = new GameGraphicsRectItem(rect);

                    item->OnLeftClick([this, i, j]() {
                        qDebug() << "Clicked on tile" << i << j;
                        qDebug() << "Current circle position is" << getCurrentCirclePosition();
                        auto from = getCurrentCirclePosition();
                        fireProjectile(from, {i, j});
                    });
                    item->OnRightClick([this, i, j]() { AddCube(i, j); });
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

        void drawCubeOnTile(int row, int col) {
            int offsetX = row * cellWidth;
            int offsetY = col * cellHeight;

            int                size = cellWidth / 2;  // You can adjust these values as you need
            QGraphicsRectItem* item = new QGraphicsRectItem(0, 0, size, size);
            item->setPos(offsetX + size / 2, offsetY + size / 2);

            qDebug() << "Adding current cube item at " << row << "," << col;
            _currentCubeItems[{row, col}] = item;
            item->setBrush(Qt::blue);
            scene->addItem(item);
        }

        void drawCubes() {
            _currentCubeItems.clear();
            for (auto& cube : _currentCubePositions) drawCubeOnTile(cube.first, cube.second);
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

            _currentCircleItem =
                new QGraphicsEllipseItem(offsetX, offsetY, circleWidth, circleHeight);
            _currentCircleItem->setBrush(qcolor);

            scene->addItem(_currentCircleItem);
        }

        int                                   rows;
        int                                   cols;
        int                                   cellWidth;
        int                                   cellHeight;
        QGraphicsScene*                       scene;
        bool                                  drawTileLines;
        std::map<std::pair<int, int>, QColor> tileColors;
    };
}