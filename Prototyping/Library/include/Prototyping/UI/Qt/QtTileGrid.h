#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "../UIPosition.h"
#include "../UISize.h"
#include "../UITileGrid.h"
#include "QtCircle.h"
#include "QtScene.h"
#include "QtView.h"

namespace Prototyping::UI::Qt {

    struct QtTileGridRenderer {
        virtual UISize     InitializeGrid() { return {}; }
        virtual UIPosition GetTileCenter(const Tile::Position& position) { return {}; }
        virtual QPolygonF  GetTileBounds(const Tile::Position& position) { return {}; }
    };

    // TODO: padding support
    class QtTileGridRectangleRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

    public:
        QtTileGridRectangleRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UISize InitializeGrid() override {
            for (uint32_t row = 0; row < _config.grid->GetRows(); row++) {
                for (uint32_t col = 0; col < _config.grid->GetColumns(); col++) {
                    if (_config.showGrid) {
                        auto rect = QRectF(
                            col * _config.tileWidth + _config.padding,
                            row * _config.tileHeight + _config.padding, _config.tileWidth,
                            _config.tileHeight
                        );
                        _scene->addRect(rect, QPen(::Qt::black));
                    }
                    if (_config.displayCoordinates) {
                        auto text = _scene->addText(
                            QString("(%1, %2)").arg(row).arg(col), QFont("Arial", 5)
                        );
                        text->setDefaultTextColor(::Qt::lightGray);
                        text->setPos(
                            col * _config.tileWidth + _config.padding,
                            row * _config.tileHeight + _config.padding
                        );
                    }
                }
            }
            return UISize{
                _config.grid->GetColumns() * _config.tileWidth + _config.padding * 2,
                _config.grid->GetRows() * _config.tileHeight + _config.padding * 2};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            return UIPosition{
                position.y * _config.tileWidth + _config.tileWidth / 2 + _config.padding / 2,
                position.x * _config.tileHeight + _config.tileHeight / 2 + _config.padding / 2};
        }
    };

    // TODO: padding support
    class QtTileGridIsometricRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;

    public:
        QtTileGridIsometricRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UISize InitializeGrid() override {
            uint32_t uiWidth    = 0;
            uint32_t uiHeight   = 0;
            qreal    tileWidth  = _config.tileWidth;
            qreal    tileHeight = _config.tileHeight;

            for (int row = 0; row < _config.grid->GetRows(); row++) {
                for (int col = 0; col < _config.grid->GetColumns(); col++) {
                    qreal x      = (col - row) * tileWidth / 2;
                    qreal y      = (row + col) * tileHeight / 2;
                    auto  top    = QPointF(x, y);
                    auto  bottom = QPointF(x, y + tileHeight);
                    auto  right  = QPointF(x + tileWidth / 2, y + tileHeight / 2);
                    auto  left   = QPointF(x - tileWidth / 2, y + tileHeight / 2);

                    QPolygonF polygon;
                    polygon << top << right << bottom << left;

                    if (uiWidth < right.x()) uiWidth = right.x();
                    if (uiHeight < bottom.y()) uiHeight = bottom.y();

                    if (_config.showGrid) {
                        QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);
                        item->setPen(QPen(::Qt::black, 1));
                        item->setBrush(::Qt::white);
                        _scene->addItem(item);
                    }
                    if (_config.displayCoordinates) {
                        auto text = _scene->addText(
                            QString("(%1, %2)").arg(row).arg(col), QFont("Arial", 5)
                        );
                        text->setDefaultTextColor(::Qt::lightGray);
                        auto center = polygon.boundingRect().center();
                        text->setPos(
                            center.x() - text->boundingRect().width() / 2,
                            center.y() - text->boundingRect().height() / 2
                        );
                    }
                }
            }
            return UISize{uiWidth, uiHeight};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            qreal tileWidth  = _config.tileWidth;
            qreal tileHeight = _config.tileHeight;
            qreal x          = (position.y - position.x) * tileWidth / 2;
            qreal y          = (position.x + position.y) * tileHeight / 2;
            auto  top        = QPointF(x, y);
            auto  bottom     = QPointF(x, y + tileHeight);
            auto  right      = QPointF(x + tileWidth / 2, y + tileHeight / 2);
            auto  left       = QPointF(x - tileWidth / 2, y + tileHeight / 2);

            QPolygonF polygon;
            polygon << top << right << bottom << left;
            auto center = polygon.boundingRect().center();

            return UIPosition{static_cast<uint32_t>(center.x()), static_cast<uint32_t>(center.y())};
        }
    };

    // TODO padding support
    class QtTileGridHexagonRenderer : public QtTileGridRenderer {
        UITileGrid::Config& _config;
        QtScene*            _scene;
        // sizes
        float hexagonSide = 20.0f;  // Length of one side of a hexagon
        float aspectRatio = 0.6f;   // Adjust this to make hexagons taller or shorter - was 0.8f
        float height      = sqrt(3.0f) * hexagonSide * aspectRatio;  // Height of a hexagon
        float width       = 2.0f * hexagonSide;                      // Width of a hexagon

    public:
        QtTileGridHexagonRenderer(UITileGrid::Config& config, QtScene* scene)
            : _config(config), _scene(scene) {}

        UISize InitializeGrid() override {
            uint32_t uiWidth    = 0;
            uint32_t uiHeight   = 0;
            qreal    tileWidth  = _config.tileWidth;   // or use width
            qreal    tileHeight = _config.tileHeight;  // or use height

            for (int row = 0; row < _config.grid->GetRows(); row++) {
                for (int col = 0; col < _config.grid->GetColumns(); col++) {
                    //////////////////////////////

                    // Calculate the top-left point of the hexagon
                    float x = col * width + ((row % 2 == 0) ? 0.0f : width / 2.0f);
                    float y = row * height * 3.0f / 4.0f;

                    // Create a hexagon
                    QPointF topLeft(x, y);
                    QPointF topRight(x + width / 2.0f, y + height / 4.0f);
                    QPointF middleRight(x + width / 2.0f, y + height * 3.0f / 4.0f);
                    QPointF bottom(x, y + height);
                    QPointF middleLeft(x - width / 2.0f, y + height * 3.0f / 4.0f);
                    QPointF topLeftExtended(x - width / 2.0f, y + height / 4.0f);

                    QPolygonF polygon;
                    polygon << topLeft << topRight << middleRight << bottom << middleLeft
                            << topLeftExtended;

                    if (uiWidth < middleRight.x()) uiWidth = middleRight.x();
                    if (uiHeight < bottom.y()) uiHeight = bottom.y();

                    if (_config.showGrid) {
                        QGraphicsPolygonItem* item = new QGraphicsPolygonItem(polygon);
                        item->setPen(QPen(::Qt::black, 1));
                        item->setBrush(::Qt::white);
                        _scene->addItem(item);
                    }
                    if (_config.displayCoordinates) {
                        auto text = _scene->addText(
                            QString("(%1, %2)").arg(row).arg(col), QFont("Arial", 5)
                        );
                        text->setDefaultTextColor(::Qt::lightGray);
                        auto center = polygon.boundingRect().center();
                        text->setPos(
                            center.x() - text->boundingRect().width() / 2,
                            center.y() - text->boundingRect().height() / 2
                        );
                    }
                }
            }
            return UISize{uiWidth, uiHeight};
        }

        UIPosition GetTileCenter(const Tile::Position& position) override {
            qreal tileWidth  = _config.tileWidth;
            qreal tileHeight = _config.tileHeight;

            // Calculate the top-left point of the hexagon
            float x = position.y * width + ((position.x % 2 == 0) ? 0.0f : width / 2.0f);
            float y = position.x * height * 3.0f / 4.0f;

            // Create a hexagon
            QPointF topLeft(x, y);
            QPointF topRight(x + width / 2.0f, y + height / 4.0f);
            QPointF middleRight(x + width / 2.0f, y + height * 3.0f / 4.0f);
            QPointF bottom(x, y + height);
            QPointF middleLeft(x - width / 2.0f, y + height * 3.0f / 4.0f);
            QPointF topLeftExtended(x - width / 2.0f, y + height / 4.0f);

            QPolygonF polygon;
            polygon << topLeft << topRight << middleRight << bottom << middleLeft
                    << topLeftExtended;

            auto center = polygon.boundingRect().center();

            return UIPosition{static_cast<uint32_t>(center.x()), static_cast<uint32_t>(center.y())};
        }
    };

    //

    class QtTileGrid : public UITileGrid {
        UITileGrid::Config _config;
        QWidget            _window;  // <-- concrete instance (* are children of this)
        QVBoxLayout        _layout;  // <-- concrete instance (* are children of this)
        QtView*            _view;
        QtScene*           _scene;
        std::unique_ptr<QtTileGridRenderer> _renderer;

    public:
        QtTileGrid(const Config& config) : _config(config) {
            _scene = new QtScene();
            _view  = new QtView(_scene);
            switch (_config.renderingStyle) {
                case RenderingStyle::Grid:
                    _renderer.reset(new QtTileGridRectangleRenderer(_config, _scene));
                    break;
                case RenderingStyle::Isometric:
                    _renderer.reset(new QtTileGridIsometricRenderer(_config, _scene));
                    break;
                case RenderingStyle::Hexagons:
                    _renderer.reset(new QtTileGridHexagonRenderer(_config, _scene));
                    break;
            }
            auto gridSize = _renderer->InitializeGrid();
            _layout.addWidget(_view);
            _window.setLayout(&_layout);
            _window.show();
        }

    protected:
        UITileGridElement* AddCircle(
            const Tile::Position& position, const UIColor& color, uint32_t diameter
        ) override {
            auto element = new QtCircle(color, diameter);
            _scene->addItem(element);
            auto center = _renderer->GetTileCenter(position);
            element->setPos(
                center.x() - static_cast<uint32_t>(diameter / 2),
                center.y() - static_cast<uint32_t>(diameter / 2)
            );
            return nullptr;  // TODO
        }
    };
}
