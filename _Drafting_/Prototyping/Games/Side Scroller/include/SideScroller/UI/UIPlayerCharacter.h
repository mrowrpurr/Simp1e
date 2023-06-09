#pragma once

#include <Animations.h>
#include <Simp1e/UI/Qt/QtUI.h>

#include <QDebug>
#include <QMovie>
#include <QPainter>
#include <QPixmap>
#include <QTimer>

#include "IUILevel.h"
#include "UILevelItem.h"

namespace SideScroller {

    class UIPlayerCharacter : public UILevelItem {
        QPixmap _lookRightImage;
        QPixmap _lookLeftImage;
        QPixmap _fallRightImage;
        QPixmap _fallLeftImage;
        QMovie* _runRightAnimation;
        QMovie* _runLeftAnimation;
        QMovie* _jumpRightAnimation;
        QMovie* _jumpLeftAnimation;

        bool _isLookingRight         = true;
        bool _isPlayingJumpAnimation = false;

        double     _playerSpeed = 50;  // Pixels per press
        LevelItem  _player;
        LevelItem* _playerPtr = &_player;

        QTimer _gravityTimer;
        int    _gravityFallSpeed = 25;  // Pixels per frame

        QTimer _leftRightTimer;
        bool   _isMovingLeft  = false;
        bool   _isMovingRight = false;

        bool   _isInAir = false;
        QTimer _jumpTimer;
        int    _jumpHeight        = 420;  // the height of the jump in pixels
        int    _jumpSpeed         = 15;   // the speed of the jump in pixels per frame
        int    _currentJumpHeight = 0;    // the current height of the jump

        void ConnectEvents() {
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

        void SetupImagesAndAnimations() {
            _lookRightImage =
                QPixmap::fromImage(
                    QImage(
                        (Animations::Root() / Animations::Filenames::STAND_RIGHT).string().c_str()
                    )
                )
                    .scaled(boundingRect().size().toSize());

            _lookLeftImage =
                QPixmap::fromImage(
                    QImage((Animations::Root() / Animations::Filenames::STAND_LEFT).string().c_str()
                    )
                )
                    .scaled(boundingRect().size().toSize());

            _fallRightImage =
                QPixmap::fromImage(
                    QImage((Animations::Root() / Animations::Filenames::FALL_RIGHT).string().c_str()
                    )
                )
                    .scaled(boundingRect().size().toSize());

            _fallLeftImage =
                QPixmap::fromImage(
                    QImage((Animations::Root() / Animations::Filenames::FALL_LEFT).string().c_str())
                )
                    .scaled(boundingRect().size().toSize());

            _runRightAnimation =
                new QMovie((Animations::Root() / Animations::Filenames::RUN_RIGHT).string().c_str()
                );
            _runRightAnimation->setScaledSize(boundingRect().size().toSize());
            _runRightAnimation->setSpeed(200);

            _runLeftAnimation =
                new QMovie((Animations::Root() / Animations::Filenames::RUN_LEFT).string().c_str());
            _runLeftAnimation->setScaledSize(boundingRect().size().toSize());
            _runLeftAnimation->setSpeed(200);

            _jumpRightAnimation =
                new QMovie((Animations::Root() / Animations::Filenames::JUMP_RIGHT).string().c_str()
                );
            _jumpRightAnimation->setScaledSize(boundingRect().size().toSize());
            _jumpRightAnimation->setSpeed(50);

            _jumpLeftAnimation =
                new QMovie((Animations::Root() / Animations::Filenames::JUMP_LEFT).string().c_str()
                );
            _jumpLeftAnimation->setScaledSize(boundingRect().size().toSize());
            _jumpLeftAnimation->setSpeed(50);

            QObject::connect(_jumpRightAnimation, &QMovie::finished, [this]() {
                _isPlayingJumpAnimation = false;
                update();
            });
            QObject::connect(_jumpLeftAnimation, &QMovie::finished, [this]() {
                _isPlayingJumpAnimation = false;
                update();
            });
        }

    public:
        UIPlayerCharacter(const LevelItem& player, IUILevel* level, QGraphicsItem* parent = nullptr)
            : UILevelItem(&_player, level, parent), _player(player) {
            ConnectEvents();
            SetupImagesAndAnimations();
        }

        ~UIPlayerCharacter() override {
            delete _runRightAnimation;
            delete _runLeftAnimation;
        }

        IUILevel*  GetLevel() const override { return UILevelItem::GetLevel(); }
        LevelItem* GetLevelItem() const override { return _playerPtr; }

    private:
        bool IsJumping() const { return _jumpTimer.isActive(); }

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
            if (_isMovingLeft) qDebug() << "... Moving left";
            if (_isMovingRight) qDebug() << "... Moving right";
            if (!_isMovingLeft && !_isMovingRight) {
                return;
            }
            if (IsAboutToCollide(Simp1e::UI::UIDirection::South, 1)) qDebug() << "... On ground";
            if (IsAboutToCollide(Simp1e::UI::UIDirection::North, 1)) qDebug() << "... On ceiling";
            if (IsAboutToCollide(Simp1e::UI::UIDirection::West, 1)) qDebug() << "... On left wall";
            if (IsAboutToCollide(Simp1e::UI::UIDirection::East, 1)) qDebug() << "... On right wall";

            if (_isMovingLeft && !IsAboutToCollide(Simp1e::UI::UIDirection::West, _playerSpeed)) {
                qDebug() << "Moving left";
                _player.position = {_player.position.x() - _playerSpeed, _player.position.y()};
                TriggerMoveCallbacks();
                prepareGeometryChange();
                update();
            } else if (_isMovingRight && !IsAboutToCollide(Simp1e::UI::UIDirection::East, _playerSpeed)) {
                qDebug() << "Moving right";
                _player.position = {_player.position.x() + _playerSpeed, _player.position.y()};
                TriggerMoveCallbacks();
                prepareGeometryChange();
                update();
            }
        }

        bool IsOnPlatformOrGround() {
            return _player.position.y() == 0 || IsAboutToCollide(Simp1e::UI::UIDirection::South, 1);
        }

        void DoGravityPlayerFalling() {
            if (IsOnPlatformOrGround()) {
                qDebug() << "On ground";
                _isPlayingJumpAnimation = false;  // This shouldn't be here...
                _isInAir                = false;
            }
            if (!IsOnPlatformOrGround()) {
                for (int i = 0; i < _gravityFallSpeed; ++i)
                    if (!IsOnPlatformOrGround() &&
                        !IsAboutToCollide(Simp1e::UI::UIDirection::South, 1))
                        _player.position = {_player.position.x(), _player.position.y() - 1};
                    else break;
                if (IsOnPlatformOrGround()) {
                    qDebug() << "On ground+";
                    _isPlayingJumpAnimation = false;  // icky
                    _isInAir                = false;
                }
                TriggerMoveCallbacks();
                prepareGeometryChange();
                update();
            }
        }

    public:
        void StartMovingLeft() override {
            _isMovingLeft   = true;
            _isLookingRight = false;
            _runLeftAnimation->start();
            qDebug() << "Start moving left";
        }

        void StopMovingLeft() override {
            _isMovingLeft = false;
            _runLeftAnimation->stop();
            qDebug() << "Stop moving left";
        }

        void StartMovingRight() override {
            _isMovingRight  = true;
            _isLookingRight = true;
            _runRightAnimation->start();
            qDebug() << "Start moving right";
        }

        void StopMovingRight() override {
            _isMovingRight = false;
            _runRightAnimation->stop();
            qDebug() << "Stop moving right";
        }

        void Jump() override {
            if (!IsOnPlatformOrGround()) return;
            if (!_jumpTimer.isActive()) _jumpTimer.start(10);
            qDebug() << "Jump";
            _isInAir = true;
            if (_isLookingRight) {
                // _runRightAnimation->stop();
                _jumpRightAnimation->stop();
                _jumpRightAnimation->start();
                _isPlayingJumpAnimation = true;
            } else {
                // _runLeftAnimation->stop();
                _jumpLeftAnimation->stop();
                _jumpLeftAnimation->start();
                _isPlayingJumpAnimation = true;
            }
        }

    protected:
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            QMovie* currentAnimation = nullptr;

            if (_isPlayingJumpAnimation && _isInAir) {
                if (_isLookingRight) currentAnimation = _jumpRightAnimation;
                else currentAnimation = _jumpLeftAnimation;
            } else if (_isMovingRight) currentAnimation = _runRightAnimation;
            else if (_isMovingLeft) currentAnimation = _runLeftAnimation;

            if (currentAnimation && currentAnimation->state() == QMovie::Running) {
                qDebug() << "Drawing animation";
                painter->drawPixmap(
                    boundingRect().toRect(), currentAnimation->currentPixmap(),
                    QRectF(
                        currentAnimation->currentFrameNumber(), 0, _player.size.width(),
                        _player.size.height()
                    )
                );
            } else {
                qDebug() << "Drawing static";
                if (_isLookingRight) {
                    if (IsOnPlatformOrGround() && !_isPlayingJumpAnimation && !_isInAir)
                        painter->drawPixmap(
                            _player.position.x(), GetItemY(), _player.size.width(),
                            _player.size.height(), _lookRightImage
                        );
                    else
                        painter->drawPixmap(
                            _player.position.x(), GetItemY(), _player.size.width(),
                            _player.size.height(), _fallRightImage
                        );
                } else {
                    if (IsOnPlatformOrGround() && !_isPlayingJumpAnimation && !_isInAir)
                        painter->drawPixmap(
                            _player.position.x(), GetItemY(), _player.size.width(),
                            _player.size.height(), _lookLeftImage
                        );
                    else
                        painter->drawPixmap(
                            _player.position.x(), GetItemY(), _player.size.width(),
                            _player.size.height(), _fallLeftImage
                        );
                }
            }
        }
    };
}
