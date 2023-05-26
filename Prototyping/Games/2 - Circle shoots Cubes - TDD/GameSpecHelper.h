#pragma once

#include <QApplication>

#include "SpecHelper.h"

namespace Prototyping {}

using namespace Prototyping;

struct SetupQtApp {
    int          argc = 0;
    char**       argv = nullptr;
    QApplication app{argc, argv};
};
