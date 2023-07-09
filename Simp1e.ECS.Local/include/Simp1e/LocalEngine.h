#pragma once

#include <Simp1e/DefaultSystemGroupTypes.h>
#include <Simp1e/Engine.h>

#include "LocalCommandManager.h"
#include "LocalEntityManager.h"
#include "LocalEventManager.h"
#include "LocalSystemGroupManager.h"

namespace Simp1e {

    class LocalEngine
        : public Engine<LocalEntityManager, LocalSystemGroupManager, LocalEventManager, LocalCommandManager> {
    public:
        void AddDefaultSystemGroups() {
            SystemGroups().AddGroup(DefaultSystemGroupTypes::InitializationGroup);
            SystemGroups().AddGroup(DefaultSystemGroupTypes::SimulationGroup);
            SystemGroups().AddGroup(DefaultSystemGroupTypes::PresentationGroup);
        }
    };
}
