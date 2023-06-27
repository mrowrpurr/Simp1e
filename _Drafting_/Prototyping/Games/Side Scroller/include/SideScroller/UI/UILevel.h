#pragma once

#include <QDebug>
#include <QGraphicsRectItem>

#include "IUIGame.h"
#include "IUILevel.h"
#include "UIPlayerCharacter.h"

namespace SideScroller {

    class UILevel : public IUILevel {
        UIPlayerCharacter*     _player;
        IUIGame*               _game;
        std::unique_ptr<Level> _level;

    public:
        UILevel(IUIGame* game, QObject* parent = nullptr) : IUILevel(parent), _game(game) {}

        std::unique_ptr<Level>& GetLevel() override { return _level; }
        IUILevelItem*           GetPlayer() override { return _player; }

    private:
        void AddItem(const LevelItem& item) { addItem(new UILevelItem(item, this)); }

        void AddItems(const std::vector<LevelItem>& items) {
            for (const auto& item : items) AddItem(item);
        }

        void AddPlayer(const PlayerCharacter& player) {
            _player = new UIPlayerCharacter(player, this);
            addItem(_player);
        }

        void AddBackground() {
            auto background = new QGraphicsRectItem(0, 0, _level->width, _level->height);
            background->setBrush(QBrush(Qt::white));
            addItem(background);
        }

    public:
        void LoadLevel(const Level& level) override {
            _level = std::make_unique<Level>(level);
            setSceneRect(0, 0, _level->width, _level->height);
            AddBackground();
            AddPlayer(_level->player);
            AddItems(_level->items);
        }
    };
}
