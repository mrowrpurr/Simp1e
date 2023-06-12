#pragma once

#include <Simp1e/UI/Qt/QtUI.h>

#include <QPainter>

#include "IUILevel.h"
#include "IUILevelItem.h"

namespace SideScroller {

    class UILevelItem : public IUILevelItem {
        LevelItem                          _ownedLevelItem;
        LevelItem*                         _levelItem;
        IUILevel*                          _level;
        std::vector<std::function<void()>> _onMoveCallbacks;

    public:
        UILevelItem(const LevelItem& levelItem, IUILevel* level, QGraphicsItem* parent = nullptr)
            : _ownedLevelItem(levelItem), _levelItem(&_ownedLevelItem), _level(level) {}

        UILevelItem(LevelItem* levelItem, IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUILevelItem(parent), _level(level), _levelItem(levelItem) {}

        IUILevel*  GetLevel() const override { return _level; }
        LevelItem* GetLevelItem() const override { return _levelItem; }

        qreal GetItemY() const {
            return _level->GetLevel()->height - _levelItem->position.y() -
                   _levelItem->size.height();
        }

        void OnMove(std::function<void()> callback) override {
            _onMoveCallbacks.push_back(callback);
        }

        // TODO
        void StartMovingLeft() override {}
        void StopMovingLeft() override {}
        void StartMovingRight() override {}
        void StopMovingRight() override {}
        void Jump() override {}

    protected:
        void TriggerMoveCallbacks() {
            for (auto& callback : _onMoveCallbacks) callback();
        }

        QRectF boundingRect() const override {
            return QRectF(
                _levelItem->position.x(), GetItemY(), _levelItem->size.width(),
                _levelItem->size.height()
            );
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            painter->setBrush(QBrush(Simp1e::UI::Qt::ToQColor(_levelItem->backgroundColor)));
            painter->setPen(QPen(Qt::white));
            painter->drawRect(
                _levelItem->position.x(), GetItemY(), _levelItem->size.width(),
                _levelItem->size.height()
            );
        }
    };
}
