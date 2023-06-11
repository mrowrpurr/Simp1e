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

        QTimer _gravityTimer;
        int    _gravityFallSpeed = 10;  // Pixels per frame

        QTimer _leftRightTimer;
        bool   _isMovingLeft  = false;
        bool   _isMovingRight = false;

        QTimer jumpTimer;
        int    jumpHeight        = 100;  // the height of the jump in pixels
        int    jumpSpeed         = 5;    // the speed of the jump in pixels per frame
        int    currentJumpHeight = 0;    // the current height of the jump

    public:
        UIPlayerCharacter(PlayerCharacter player, IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUIPlayerCharacter(parent), _player(player), _level(level) {
            //
            jumpTimer.setInterval(20);  // <--- changing this doens't seem to do anything
            QObject::connect(&jumpTimer, &QTimer::timeout, [this]() { jumpFrame(); });
            //
            QObject::connect(&_leftRightTimer, &QTimer::timeout, [this]() {
                DoLeftRightMovement();
            });
            _leftRightTimer.start(50);
            //
            QObject::connect(&_gravityTimer, &QTimer::timeout, [this]() {
                DoGravityPlayerFalling();
            });
            _gravityTimer.start(50);
        }

        LevelItem& GetLevelItem() override { return _player; }

    private:
        void jumpFrame() {
            if (currentJumpHeight < jumpHeight &&
                !IsAboutToCollide(Simp1e::UI::UIDirection::North, jumpSpeed)) {
                _player.position = {_player.position.x(), _player.position.y() + jumpSpeed};
                currentJumpHeight += jumpSpeed;
            } else {
                jumpTimer.stop();
                currentJumpHeight = 0;
            }

            prepareGeometryChange();
            update();
        }

        void DoLeftRightMovement() {
            if (_isMovingLeft && !IsAboutToCollide(Simp1e::UI::UIDirection::West, _playerSpeed)) {
                qDebug() << "Moving left";
                _player.position = {_player.position.x() - _playerSpeed, _player.position.y()};
                prepareGeometryChange();
                update();
            } else if (_isMovingRight && !IsAboutToCollide(Simp1e::UI::UIDirection::East, _playerSpeed)) {
                qDebug() << "Moving right";
                _player.position = {_player.position.x() + _playerSpeed, _player.position.y()};
                prepareGeometryChange();
                update();
            }
        }

        bool IsOnPlatformOrGround() {
            return _player.position.y() == 0 || IsAboutToCollide(Simp1e::UI::UIDirection::South, 1);
        }

        void DoGravityPlayerFalling() {
            if (!IsOnPlatformOrGround()) {
                _player.position = {_player.position.x(), _player.position.y() - _gravityFallSpeed};
                if (_player.position.y() < 0) _player.position = {_player.position.x(), 0.0};
                prepareGeometryChange();
                update();
            }
        }

    public:
        IUILevel*        GetLevel() override { return _level; }
        PlayerCharacter& GetPlayer() override { return _player; }

        qreal GetPlayerY() const {
            return _level->GetLevel()->height - _player.position.y() - _player.size.height();
        }

        void StartMovingLeft() override {
            _isMovingLeft = true;
            qDebug() << "Start moving left";
        }

        void StopMovingLeft() override {
            _isMovingLeft = false;
            qDebug() << "Stop moving left";
        }

        void StartMovingRight() override {
            _isMovingRight = true;
            qDebug() << "Start moving right";
        }

        void StopMovingRight() override {
            _isMovingRight = false;
            qDebug() << "Stop moving right";
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
