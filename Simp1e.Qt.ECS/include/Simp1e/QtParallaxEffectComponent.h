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
            Position                             position;
            std::unique_ptr<QGraphicsPixmapItem> pixmapItem;

            LayerTile(const Position& position, QPixmap& pixmap, IParallaxEffectLayer* layer) : position(position) {
                pixmapItem = std::make_unique<QGraphicsPixmapItem>(pixmap);
                pixmapItem->setPos(ToQPointF(position));
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
            std::unordered_set<Position>            tilesPositions;
        };

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

        void _findOrCreateTile(const Position& desiredPosition, LayerInfo& layerInfo, IParallaxEffectLayer* layer) {
            for (auto& tile : layerInfo.tiles)
                if (tile->position == desiredPosition) return;
            auto& tile =
                layerInfo.tiles.emplace_back(std::make_unique<LayerTile>(desiredPosition, layerInfo.pixmap, layer));
            _scene->addItem(tile->pixmapItem.get());
            layerInfo.tilesPositions.insert(desiredPosition);
        }

        void _updateAllTilePositions(const Position& positionDelta, LayerInfo& layerInfo) {
            for (auto& tile : layerInfo.tiles) {
                layerInfo.tilesPositions.erase(tile->position);
                tile->position.SetX(tile->position.x() + positionDelta.x());
                tile->position.SetY(tile->position.y() + positionDelta.y());
                tile->pixmapItem->setPos(ToQPointF(tile->position));
                layerInfo.tilesPositions.insert(tile->position);
            }
        }

        void _addTilesToFillViewport(
            const Position& desiredPosition, const Rectangle& viewport, LayerInfo& layerInfo,
            IParallaxEffectLayer* layer
        ) {
            if (layerInfo.tilesPositions.find(desiredPosition) != layerInfo.tilesPositions.end()) return;

            Rectangle desiredRect(desiredPosition, Size(layerInfo.pixmap.width(), layerInfo.pixmap.height()));
            if (!viewport.intersects(desiredRect)) return;

            _Log_("----");
            _Log_("ADDING. Currently there are {} tiles", layerInfo.tiles.size());
            for (auto& tile : layerInfo.tiles) {
                Rectangle tileRect(tile->position, Size(layerInfo.pixmap.width(), layerInfo.pixmap.height()));
                _Log_("Tile:{}", tileRect.ToString());
            }
            for (auto& position : layerInfo.tilesPositions) _Log_("KNOWN TilePosition:{}", position.ToString());
            _Log_("----");

            _Log_(
                "Adding! Apparently Desired Rect {} intersects the viewport {}", desiredRect.ToString(),
                viewport.ToString()
            );

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
                Rectangle tileRect((*it)->position, Size(layerInfo.pixmap.width(), layerInfo.pixmap.height()));
                _Log_("Should we remove? TileRect:{} Viewport:{}", tileRect.ToString(), viewport.ToString());
                if (!viewport.intersects(tileRect)) {
                    _Log_("Removing Tile:{}", (*it)->position.ToString());
                    it = layerInfo.tiles.erase(it);
                    layerInfo.tilesPositions.erase((*it)->position);
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
            auto&     layerInfo = _getLayerInfo(layer);
            Position  positionDelta(
                (viewportTopLeft.x() - layerInfo.lastPosition.x()) * layer->speed(),
                (viewportTopLeft.y() - layerInfo.lastPosition.y()) * layer->speed()
            );
            layerInfo.lastPosition = viewportTopLeft;

            _Log_("CONFIGURE LAYER - there are currently {} tiles", layerInfo.tiles.size());

            _updateAllTilePositions(positionDelta, layerInfo);

            // Must be done after updating tile positions
            auto startingPosition = layerInfo.tiles.empty() ? viewportTopLeft : layerInfo.tiles.front()->position;

            _addTilesToFillViewport(startingPosition, viewport, layerInfo, layer);
            // _removeTilesOutsideOfViewport(viewport, layerInfo);
        }
    };
}
