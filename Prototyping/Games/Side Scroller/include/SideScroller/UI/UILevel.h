#pragma once

#include <QGraphicsRectItem>

#include "IUIGame.h"
#include "IUILevel.h"

namespace SideScroller {

    class UILevel : public IUILevel {
        IUIGame* _game;

    public:
        UILevel(IUIGame* game, QObject* parent = nullptr) : IUILevel(parent), _game(game) {}
        void LoadLevel(const Level& level) override {
            auto rect = new QGraphicsRectItem{
                0, 0, static_cast<qreal>(level.width), static_cast<qreal>(level.height)};

            // for (const auto& item : level.items) {
            // auto rect = new QGraphicsRectItem{item.position.x, item.position.y,
            // item.size.width, item.size.height};
            // rect->setBrush(QBrush{QColor{item.backgroundColor.r, item.backgroundColor.g,
            // item.backgroundColor.b}}); addItem(rect);
            // }
        }
    };
}
