#pragma once

#include <Simp1e/UI/UIColor.h>
#include <Simp1e/UI/UILineStyle.h>

#include <QColor>
#include <QPen>

namespace Simp1e::UI::Qt {
    QColor ToQColor(const UIColor& color) {
        return QColor(color.red, color.green, color.blue, color.alpha);
    }

    ::Qt::PenStyle ToQtPenStyle(UILineStyle style) {
        switch (style) {
            case UILineStyle::Solid:
                return ::Qt::SolidLine;
            case UILineStyle::Dashed:
                return ::Qt::DashLine;
            case UILineStyle::Dotted:
                return ::Qt::DotLine;
            case UILineStyle::None:
                return ::Qt::NoPen;
            default:
                return ::Qt::SolidLine;
        }
    }
}
