#pragma once

#include <QPainter>

#include "IUILevel.h"
#include "IUIPlayerCharacter.h"
#include "UILevelItem.h"

namespace SideScroller {

    class UIPlayerCharacter : public IUIPlayerCharacter {
        IUILevel*       _level;
        PlayerCharacter _player;

    public:
        UIPlayerCharacter(PlayerCharacter player, IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUIPlayerCharacter(parent), _player(player), _level(level) {}

        IUILevel* GetLevel() override { return _level; }

        qreal GetPlayerY() const {
            return _level->GetLevel()->height - _player.position.y() - _player.size.height();
        }

    protected:
        QRectF boundingRect() const override {
            return QRectF(
                _player.position.x(), GetPlayerY(), _player.size.width(), _player.size.height()
            );
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            painter->setBrush(QBrush(Qt::magenta));
            painter->setPen(QPen(Qt::white));
            painter->drawRect(
                _player.position.x(), GetPlayerY(), _player.size.width(), _player.size.height()
            );
        }
    };
}
