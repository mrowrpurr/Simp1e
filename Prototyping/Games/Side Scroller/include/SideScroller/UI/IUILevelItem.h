#pragma once

#include <Simp1e/UI/UIDirection.h>

#include <QGraphicsItem>

#include "../Game/LevelItem.h"

namespace SideScroller {

    struct IUILevel;

    struct IUILevelItem : public QGraphicsItem {
        IUILevelItem(QGraphicsItem* parent = nullptr) : QGraphicsItem(parent) {}
        virtual ~IUILevelItem()           = default;
        virtual IUILevel*  GetLevel()     = 0;
        virtual LevelItem& GetLevelItem() = 0;

    protected:
        QRectF boundingRect() const override { return QRectF(); }
        void   paint(QPainter*, const QStyleOptionGraphicsItem*, QWidget*) override {}

        bool IsAboutToCollide(Simp1e::UI::UIDirection direction, double speed = 1) {
            // Ignore the diagonals and just look at Up(North), Down(South), Left(West), Right(East)
            QRectF nextBoundingRect = boundingRect();

            switch (direction) {
                case Simp1e::UI::UIDirection::North:
                    nextBoundingRect.translate(0, -speed);  // Negative because we're going up
                    break;
                case Simp1e::UI::UIDirection::South:
                    nextBoundingRect.translate(0, speed);
                    break;
                case Simp1e::UI::UIDirection::West:
                    nextBoundingRect.translate(-speed, 0);
                    break;
                case Simp1e::UI::UIDirection::East:
                    nextBoundingRect.translate(speed, 0);
                    break;
                default:
                    break;
            }

            for (auto& item : scene()->items(nextBoundingRect)) {
                if (item == this) continue;
                if (auto* levelItem = dynamic_cast<IUILevelItem*>(item))
                    if (levelItem->GetLevelItem().type == LevelItemType::Platform) return true;
            }

            return false;
        }
    };
}
