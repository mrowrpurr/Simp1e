#pragma once

#include <Simp1e/Engine.h>

#include "LocalCommandManager.h"
#include "LocalEntityManager.h"
#include "LocalEventManager.h"
#include "LocalSystemManager.h"

namespace Simp1e {

    class LocalEngine : public Engine<LocalEntityManager, LocalSystemManager, LocalEventManager, LocalCommandManager> {
    };
}
