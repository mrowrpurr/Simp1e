#pragma once

#include <QGraphicsScene>
#include <memory>

#include "../Game/Level.h"
#include "IUILevelItem.h"

namespace SideScroller {

    class IUILevel : public QGraphicsScene {
    public:
        IUILevel(QObject* parent = nullptr) : QGraphicsScene(parent) {}
        virtual ~IUILevel()                                     = default;
        virtual void                    LoadLevel(const Level&) = 0;
        virtual std::unique_ptr<Level>& GetLevel()              = 0;
        virtual IUILevelItem*           GetPlayer()             = 0;
    };
}
