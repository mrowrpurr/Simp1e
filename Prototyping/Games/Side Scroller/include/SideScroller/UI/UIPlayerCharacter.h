#pragma once

#include <Simp1e/UI/Qt/QtUI.h>

#include <QDebug>
#include <QPainter>
#include <QTimer>

#include "IUILevel.h"
#include "IUIPlayerCharacter.h"
#include "UILevelItem.h"

namespace SideScroller {

    class UIPlayerCharacter : public IUIPlayerCharacter {
        double          _playerSpeed = 10;  // Pixels per press
        IUILevel*       _level;
        PlayerCharacter _player;

        QTimer jumpTimer;
        int    jumpHeight        = 200;  // the height of the jump in pixels
        int    jumpSpeed         = 3;    // the speed of the jump in pixels per frame
        int    currentJumpHeight = 0;    // the current height of the jump

    public:
        UIPlayerCharacter(PlayerCharacter player, IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUIPlayerCharacter(parent), _player(player), _level(level) {
            jumpTimer.setInterval(20);  // <--- changing this doens't seem to do anything
            QObject::connect(&jumpTimer, &QTimer::timeout, [this]() { jumpFrame(); });
        }

    private:
        void jumpFrame() {
            if (currentJumpHeight < jumpHeight / 2) {
                // We're still going up
                // _player.position.y() -= jumpSpeed;
                _player.position = {_player.position.x(), _player.position.y() + jumpSpeed};
                currentJumpHeight += jumpSpeed;
            } else if (currentJumpHeight < jumpHeight) {
                // We're coming down
                // _player.position.y() += jumpSpeed;
                _player.position = {_player.position.x(), _player.position.y() - jumpSpeed};
                currentJumpHeight += jumpSpeed;
            } else {
                // We've finished the jump
                jumpTimer.stop();
                currentJumpHeight = 0;
            }

            prepareGeometryChange();
            update();
        }

    public:
        IUILevel*        GetLevel() override { return _level; }
        PlayerCharacter& GetPlayer() override { return _player; }

        qreal GetPlayerY() const {
            return _level->GetLevel()->height - _player.position.y() - _player.size.height();
        }

        void MoveLeft() override {
            qDebug() << "Move left";
            prepareGeometryChange();
            _player.position = {_player.position.x() - _playerSpeed, _player.position.y()};
            update();
        }

        void MoveRight() override {
            qDebug() << "Move right";
            prepareGeometryChange();
            _player.position = {_player.position.x() + _playerSpeed, _player.position.y()};
            update();
        }

        void Jump() override {
            if (!jumpTimer.isActive()) jumpTimer.start(10);
        }

    protected:
        QRectF boundingRect() const override {
            return QRectF(
                _player.position.x(), GetPlayerY(), _player.size.width(), _player.size.height()
            );
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)

            override {
            qDebug() << "Painting player";
            painter->setBrush(QBrush(Simp1e::UI::Qt::ToQColor(_player.backgroundColor)));
            painter->setPen(QPen(Qt::white));
            painter->drawRect(
                _player.position.x(), GetPlayerY(), _player.size.width(), _player.size.height()
            );
        }
    };
}
