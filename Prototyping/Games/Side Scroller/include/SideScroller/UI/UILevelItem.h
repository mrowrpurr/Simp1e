#pragma once

#include <QPainter>

#include "IUILevel.h"
#include "IUILevelItem.h"

namespace SideScroller {

    class UILevelItem : public IUILevelItem {
        LevelItem _levelItem;
        IUILevel* _level;

    public:
        UILevelItem(const LevelItem& levelItem, IUILevel* level, QGraphicsItem* parent = nullptr)
            : IUILevelItem(parent), _level(level), _levelItem(levelItem) {}

        IUILevel* GetLevel() override { return _level; }

        qreal GetItemY() const {
            return _level->GetLevel()->height - _levelItem.position.y() - _levelItem.size.height();
        }

    protected:
        QRectF boundingRect() const override {
            return QRectF(
                _levelItem.position.x(), GetItemY(), _levelItem.size.width(),
                _levelItem.size.height()
            );
        }

        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
            override {
            painter->setBrush(QBrush(Qt::green));
            painter->setPen(QPen(Qt::white));
            painter->drawRect(
                _levelItem.position.x(), GetItemY(), _levelItem.size.width(),
                _levelItem.size.height()
            );
        }
    };
}
