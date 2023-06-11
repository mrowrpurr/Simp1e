#pragma once

#include <QDebug>
#include <QGraphicsRectItem>

#include "IUIGame.h"
#include "IUILevel.h"

namespace SideScroller {

    class UILevel : public IUILevel {
        IUIGame*               _game;
        std::unique_ptr<Level> _level;

    public:
        UILevel(IUIGame* game, QObject* parent = nullptr) : IUILevel(parent), _game(game) {}

        void LoadLevel(const Level& level) override {
            _level = std::make_unique<Level>(level);

            // // Make full size background rect
            // auto backgroundRect = new QGraphicsRectItem(0, 0, level.width, level.height);
            // backgroundRect->setBrush(QBrush(Qt::white));
            // addItem(backgroundRect);

            // // Hmm make a small item all the way at the right
            auto rect = new QGraphicsRectItem(0, 0, 5, 5);
            rect->setPos(level.width - 5, level.height - 5);
            rect->setBrush(QBrush(Qt::red));
            addItem(rect);

            // for (const auto& item : level.items) {
            // HERE!

            // auto rect = new QGraphicsRectItem(0, 0, 100, 100);
            // rect->setBrush(QBrush(Qt::red));
            // addItem(rect);

            auto anotherRect = new QGraphicsRectItem(100, 100, 100, 100);
            anotherRect->setBrush(QBrush(Qt::blue));
            addItem(anotherRect);

            // setSceneRect(0, 0, level.width + 10, level.height + 10);
        }

        std::unique_ptr<Level>& GetLevel() override { return _level; }
    };
}
