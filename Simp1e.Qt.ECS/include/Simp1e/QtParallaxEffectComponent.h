#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/IParallaxEffectComponent.h>
#include <Simp1e/PositionHashKeySupport.h>
#include <Simp1e/Rectangle.h>
#include <Simp1e/Size.h>
#include <Simp1e/ToQPointF.h>
#include <_Log_.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QPixmap>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace Simp1e {

    class QtParallaxEffectComponent {
        QGraphicsScene* _scene;

        struct LayerTile {
            Rectangle                            rect;
            std::unique_ptr<QGraphicsPixmapItem> pixmapItem;

            LayerTile(const Rectangle& rect, QPixmap& pixmap, IParallaxEffectLayer* layer) : rect(rect) {
                pixmapItem = std::make_unique<QGraphicsPixmapItem>(pixmap);
                pixmapItem->setPos(ToQPointF(rect.topLeft().ToPoint()));
                pixmapItem->setZValue(-100);  // TODO: get the index!
                pixmapItem->setOpacity(layer->opacity());
            }

            ~LayerTile() {
                if (pixmapItem) pixmapItem->scene()->removeItem(pixmapItem.get());
            }
        };

        struct LayerInfo {
            QPixmap     pixmap;
            Position    lastPosition;
            std::string lastImageName;  // TODO: Use this to detect when to update the pixmap
            std::vector<std::unique_ptr<LayerTile>> tiles;
        };

        std::unordered_set<Position>               _checkedPositionsForAddingTiles;
        std::unordered_map<std::string, LayerInfo> _layerInfo;

        LayerInfo& _getLayerInfo(IParallaxEffectLayer* layer) {
            auto found = _layerInfo.find(layer->layerName());
            if (found != _layerInfo.end()) return found->second;
            auto& layerInfo  = _layerInfo[layer->layerName()];
            layerInfo.pixmap = QPixmap(layer->imagePath());
            if (layer->scale() != 1.0)
                layerInfo.pixmap = layerInfo.pixmap.scaled(
                    layerInfo.pixmap.width() * layer->scale(), layerInfo.pixmap.height() * layer->scale()
                );
            return layerInfo;
        }

        bool _tileIntersectsAnyExistingLayer(const Rectangle& rect, LayerInfo& layerInfo) {
            for (auto& tile : layerInfo.tiles)
                if (rect == tile->rect || tile->rect.intersectsInside(rect)) return true;
            return false;
        }

        void _findOrCreateTile(const Position& desiredPosition, LayerInfo& layerInfo, IParallaxEffectLayer* layer) {
            if (_checkedPositionsForAddingTiles.find(desiredPosition) != _checkedPositionsForAddingTiles.end()) return;
            _checkedPositionsForAddingTiles.insert(desiredPosition);

            Rectangle desiredRect(desiredPosition, Size(layerInfo.pixmap.width(), layerInfo.pixmap.height()));
            if (_tileIntersectsAnyExistingLayer(desiredRect, layerInfo)) return;

            _Log_("ADD {}", desiredRect.ToString());

            auto& tile =
                layerInfo.tiles.emplace_back(std::make_unique<LayerTile>(desiredRect, layerInfo.pixmap, layer));
            _scene->addItem(tile->pixmapItem.get());
        }

        void _updateAllTilePositions(const Position& positionDelta, LayerInfo& layerInfo) {
            for (auto& tile : layerInfo.tiles) {
                tile->rect.SetOrigin(tile->rect.origin() + positionDelta.ToPoint());
                tile->pixmapItem->setPos(ToQPointF(tile->rect.topLeft().ToPoint()));
            }
        }

        void _addTilesToFillViewport(
            const Position& desiredPosition, const Rectangle& viewport, LayerInfo& layerInfo,
            IParallaxEffectLayer* layer
        ) {
            if (_checkedPositionsForAddingTiles.find(desiredPosition) != _checkedPositionsForAddingTiles.end()) return;
            // _Log_("_addTilesToFillViewport: {}", desiredPosition.ToString());

            Rectangle desiredRect(desiredPosition, Size(layerInfo.pixmap.width(), layerInfo.pixmap.height()));
            if (!viewport.intersects(desiredRect)) return;

            _findOrCreateTile(desiredPosition, layerInfo, layer);

            _addTilesToFillViewport(
                Position(desiredPosition.x(), desiredPosition.y() - layerInfo.pixmap.height()), viewport, layerInfo,
                layer
            );
            _addTilesToFillViewport(
                Position(desiredPosition.x(), desiredPosition.y() + layerInfo.pixmap.height()), viewport, layerInfo,
                layer
            );
            _addTilesToFillViewport(
                Position(desiredPosition.x() - layerInfo.pixmap.width(), desiredPosition.y()), viewport, layerInfo,
                layer
            );
            _addTilesToFillViewport(
                Position(desiredPosition.x() + layerInfo.pixmap.width(), desiredPosition.y()), viewport, layerInfo,
                layer
            );
        }

        void _removeTilesOutsideOfViewport(const Rectangle& viewport, LayerInfo& layerInfo) {
            for (auto it = layerInfo.tiles.begin(); it != layerInfo.tiles.end();) {
                if (!viewport.intersects((*it)->rect) && !viewport.contains((*it)->rect)) {
                    _Log_(
                        "REMOVE {} (because it doesn't intersect with {})", (*it)->rect.ToString(), viewport.ToString()
                    );
                    it = layerInfo.tiles.erase(it);
                } else ++it;
            }
        }

    public:
        DEFINE_COMPONENT_TYPE("QtParallaxEffect")

        QtParallaxEffectComponent(QGraphicsScene* scene) : _scene(scene) {}

        void ConfigureLayer(
            int index, const Size& viewportSize, const Position& viewportTopLeft, IParallaxEffectLayer* layer
        ) {
            Rectangle viewport(viewportTopLeft, viewportSize);
            _Log_("TOP LEFT: {}", viewportTopLeft.ToString());
            _Log_("VIEWPORT: {}", viewport.ToString());
            auto&    layerInfo = _getLayerInfo(layer);
            Position newBasePosition(
                (viewportTopLeft.x() - layerInfo.lastPosition.x()) * layer->speed(),
                (viewportTopLeft.y() - layerInfo.lastPosition.y()) * layer->speed()
            );
            layerInfo.lastPosition = viewportTopLeft;

            _updateAllTilePositions(newBasePosition, layerInfo);

            _Log_("CONFIGURE LAYER - there are currently {} tiles", layerInfo.tiles.size());
            for (auto& tile : layerInfo.tiles) {
                _Log_("TILE: {}", tile->rect.ToString());
            }

            auto      width  = layerInfo.pixmap.width();
            auto      height = layerInfo.pixmap.height();
            Rectangle viewportWithPadding(
                Position(viewportTopLeft.x() - width, viewportTopLeft.y() - height),
                Size(viewportSize.width() + width * 2, viewportSize.height() + height * 2)
            );

            // Must be done after updating tile positions
            _checkedPositionsForAddingTiles.clear();
            if (layerInfo.tiles.empty()) _findOrCreateTile(viewportTopLeft, layerInfo, layer);

            auto startingPosition = layerInfo.tiles.front()->rect.topLeft();

            _Log_("Add Tiles To Fill Viewport from starting position {}", startingPosition.ToString());
            _addTilesToFillViewport(startingPosition, viewportWithPadding, layerInfo, layer);
            _removeTilesOutsideOfViewport(viewport, layerInfo);

            _Log_("CONFIGURE LAYER - there are now {} tiles", layerInfo.tiles.size());

            _scene->update();
        }
    };
}
