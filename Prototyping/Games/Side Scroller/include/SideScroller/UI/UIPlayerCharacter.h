#pragma once

#include <Simp1e/UI/Qt/QtUI.h>

#include <QDebug>
#include <QPainter>
#include <QTimer>

#include "IUILevel.h"
#include "UILevelItem.h"

namespace SideScroller {

    class UIPlayerCharacter : public UILevelItem {
        double     _playerSpeed = 10;  // Pixels per press
        LevelItem  _player;
        LevelItem* _playerPtr = &_player;

        QTimer _gravityTimer;
        int    _gravityFallSpeed = 10;  // Pixels per frame

        QTimer _leftRightTimer;
        bool   _isMovingLeft  = false;
        bool   _isMovingRight = false;

        QTimer _jumpTimer;
        int    _jumpHeight        = 100;  // the height of the jump in pixels
        int    _jumpSpeed         = 5;    // the speed of the jump in pixels per frame
        int    _currentJumpHeight = 0;    // the current height of the jump

    public:
        UIPlayerCharacter(const LevelItem& player, IUILevel* level, QGraphicsItem* parent = nullptr)
            : UILevelItem(&_player, level, parent), _player(player) {
            // Connect the jump timer
            QObject::connect(&_jumpTimer, &QTimer::timeout, [this]() { jumpFrame(); });
            // Connect the left/right timer
            QObject::connect(&_leftRightTimer, &QTimer::timeout, [this]() {
                DoLeftRightMovement();
            });
            _leftRightTimer.start(50);
            // Connect the gravity timer
            QObject::connect(&_gravityTimer, &QTimer::timeout, [this]() {
                DoGravityPlayerFalling();
            });
            _gravityTimer.start(50);
        }

        IUILevel*  GetLevel() const override { return UILevelItem::GetLevel(); }
        LevelItem* GetLevelItem() const override { return _playerPtr; }

    private:
        void jumpFrame() {
            if (_currentJumpHeight < _jumpHeight &&
                !IsAboutToCollide(Simp1e::UI::UIDirection::North, _jumpSpeed)) {
                _player.position = {_player.position.x(), _player.position.y() + _jumpSpeed};
                _currentJumpHeight += _jumpSpeed;
            } else {
                _jumpTimer.stop();
                _currentJumpHeight = 0;
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
            if (!IsOnPlatformOrGround()) return;
            if (!_jumpTimer.isActive()) _jumpTimer.start(10);
        }
    };
}
