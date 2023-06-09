#pragma once

#include <Simp1e/DefineComponentType.h>

#include <QMenu>

namespace Simp1e {

    class QtMenuComponent {
        QMenu* _menu;

    public:
        DEFINE_COMPONENT_TYPE("QMenu")

        QtMenuComponent(QMenu* menu) : _menu(menu) {}

        QMenu* GetQMenu() const { return _menu; }
    };
}
