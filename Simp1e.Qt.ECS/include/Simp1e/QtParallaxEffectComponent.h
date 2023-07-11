#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/IParallaxEffectComponent.h>
#include <Simp1e/Size.h>
#include <Simp1e/ToQPointF.h>
#include <_Log_.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <unordered_map>

namespace Simp1e {

    class QtParallaxEffectComponent {
        QGraphicsScene* _scene;

        struct TileData {
            QGraphicsPixmapItem* item;
            Position             position;  // The tile's position in the world
        };

        // This map associates each layer with its set of tiles
        std::unordered_map<IParallaxEffectLayer*, std::vector<TileData>> layerToTiles;

    public:
        DEFINE_COMPONENT_TYPE("QtParallaxEffect")

        QtParallaxEffectComponent(QGraphicsScene* scene) : _scene(scene) {}

        // Index is the index of the layer, e.g. 0, 1, 2... I'm using it for z-index
        // Size is the size of the viewport
        // The IParallaxEffectLayer is the layer data which includes: name, path, speed, scale, opacity
        // viewportTopLeft is the current position of the top left of the perspective
        //
        // This function is called every frame (if the player moved) and is responsible for updating the
        // position of the layer items. It's also responsible for creating the layer items if they don't
        // exist yet.
        //
        // The QGraphicsScene is available in the _scene member variable.
        void ConfigureLayer(
            int index, const Size& viewportSize, const Position& viewportTopLeft, IParallaxEffectLayer* layer
        ) {
            // TODO add some logic

            // Just make an item for now to see SOMETHING ...
            // The following code just adds this layer to the scene so we can see it.
            // auto* item = new QGraphicsPixmapItem(QPixmap(layer->imagePath()));
            // if (layer->opacity() != 1.0) item->setOpacity(layer->opacity());
            // if (layer->scale() != 1.0) item->setScale(layer->scale());
            // item->setZValue(-100 + index);
            // item->setPos({-500, -500});
            // _Log_(
            //     "LAYER {} width:{} height:{}", layer->layerName(), item->boundingRect().width(),
            //     item->boundingRect().height()
            // );
            // _scene->addItem(item);
            ///

            // Calculate the size of the tiles
            QPixmap pixmap(layer->imagePath());
            QSize   tileSize = pixmap.size() * layer->scale();

            // Calculate the number of tiles required to cover the viewport in each dimension
            int numTilesX = viewportSize.width() / tileSize.width() + 2;    // +2 for the buffer tiles
            int numTilesY = viewportSize.height() / tileSize.height() + 2;  // +2 for the buffer tiles

            // Calculate the start position for the tile grid. This position will always be at the top left of the
            // viewport. We offset the position based on the layer's speed to create the parallax effect.
            Position startPos = viewportTopLeft * layer->speed();
            startPos.SetX(std::floor(startPos.x() / tileSize.width()) * tileSize.width());
            startPos.SetY(std::floor(startPos.y() / tileSize.height()) * tileSize.height());

            // Fetch the existing tiles for this layer
            std::vector<TileData>& tiles = layerToTiles[layer];

            // Remove tiles that are outside the new grid
            tiles.erase(
                std::remove_if(
                    tiles.begin(), tiles.end(),
                    [&](const TileData& tile) {
                        qreal distX = std::abs(tile.position.x() - viewportTopLeft.x());
                        qreal distY = std::abs(tile.position.y() - viewportTopLeft.y());
                        return distX > numTilesX * tileSize.width() || distY > numTilesY * tileSize.height();
                    }
                ),
                tiles.end()
            );

            // Create the new grid of tiles
            for (int i = 0; i < numTilesX; ++i) {
                for (int j = 0; j < numTilesY; ++j) {
                    Position tilePos =
                        startPos +
                        Position{static_cast<sreal>(i * tileSize.width()), static_cast<sreal>(j * tileSize.height())};

                    // Check if a tile already exists at this position
                    auto it = std::find_if(tiles.begin(), tiles.end(), [&](const TileData& tile) {
                        return tile.position == tilePos;
                    });

                    if (it != tiles.end()) {
                        // A tile already exists at this position, so just move it
                        it->item->setPos(ToQPointF(tilePos.ToPoint()));
                    } else {
                        // No tile exists at this position, so create a new one
                        auto* item = new QGraphicsPixmapItem(pixmap);
                        item->setPos(ToQPointF(tilePos.ToPoint()));
                        item->setScale(layer->scale());
                        item->setOpacity(layer->opacity());
                        item->setZValue(-100 + index);
                        _scene->addItem(item);

                        tiles.push_back({item, tilePos});
                    }
                }
            }
        }
    };
}
