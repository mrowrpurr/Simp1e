#pragma once

#include <Simp1e/DefineComponentType.h>

#include <QWidget>
#include <memory>

namespace Simp1e {

    class QWidgetComponent {
        QWidget* _widget;

    public:
        DEFINE_COMPONENT_TYPE("QWidget")

        QWidgetComponent(QWidget* widget) : _widget(widget) {}

        QWidget* GetQWidget() const { return _widget; }
    };
}
