#pragma once

#include <QApplication>

#include "Prototyping/GameCircleCube.h"
#include "Prototyping/GameCircleCubeInputHandler.h"
#include "SpecHelper.h"

using namespace Prototyping;

struct SetupQtApp {
    int          argc = 0;
    char**       argv = nullptr;
    QApplication app{argc, argv};
};
