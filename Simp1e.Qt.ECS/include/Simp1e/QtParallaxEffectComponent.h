#pragma once

#include <Simp1e/DefineComponentType.h>
#include <Simp1e/IParallaxEffectComponent.h>
#include <_Log_.h>

#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <unordered_map>

namespace Simp1e {

    class QtParallaxEffectComponent {
        QGraphicsScene* _scene;

    public:
        DEFINE_COMPONENT_TYPE("QtParallaxEffect")

        QtParallaxEffectComponent(QGraphicsScene* scene) : _scene(scene) {}

        void ConfigureLayer(IParallaxEffectLayer* layer, const Position& currentPerspectivePosition) {
            _Log_(
                "LAYER NAME: {} should support position {}", layer->layerName(), currentPerspectivePosition.ToString()
            );

            // Just make an item for now to see SOMETHING ...
            auto* item = new QGraphicsPixmapItem(QPixmap(layer->imagePath()));
            if (layer->opacity() != 1.0) item->setOpacity(layer->opacity());
            if (layer->scale() != 1.0) item->setScale(layer->scale());
            item->setZValue(-1);
            _scene->addItem(item);
        }
    };
}
