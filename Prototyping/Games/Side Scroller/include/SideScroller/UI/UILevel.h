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

            // Create 1 rectangle for the entire background for the entire level scene
            auto background = new QGraphicsRectItem{0, 0, level.width, level.height};
            qDebug() << "UILevel LoadLevel Level size: " << level.width << "x" << level.height;
            background->setBrush(QBrush{
                QColor{0, 255, 255}
            });
            addItem(background);

            auto rect = new QGraphicsRectItem{0, 0, 300, 400};
            rect->setPos(300, 600);
            qDebug() << "Pink rect x y width height: " << rect->x() << " " << rect->y() << " "
                     << rect->rect().width() << " " << rect->rect().height();
            rect->setBrush(QBrush{
                QColor{255, 0, 255}
            });
            addItem(rect);

            // for (const auto& item : level.items) {
        }

        std::unique_ptr<Level>& GetLevel() override { return _level; }
    };
}
