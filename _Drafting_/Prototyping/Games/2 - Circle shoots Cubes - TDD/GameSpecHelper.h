#pragma once

#include <QApplication>

#include "SpecHelper.h"

namespace Simp1e {}

using namespace Simp1e;

struct SetupQtApp {
    int          argc = 0;
    char**       argv = nullptr;
    QApplication app{argc, argv};
};
