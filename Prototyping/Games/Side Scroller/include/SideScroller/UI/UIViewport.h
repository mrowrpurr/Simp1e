#pragma once

#include "IUIViewport.h"

namespace SideScroller {

    class UIViewport : public IUIViewport {
    public:
        UIViewport(QWidget* parent = nullptr) : IUIViewport(parent) {}
    };
}
