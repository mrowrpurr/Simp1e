#pragma once

#include <Simp1e/DefineComponentType.h>

#include <QAction>

namespace Simp1e {

    class QActionComponent {
        QAction* _action;

    public:
        DEFINE_COMPONENT_TYPE("QAction")

        QActionComponent(QAction* action) : _action(action) {}

        QAction* GetQAction() const { return _action; }
    };
}
