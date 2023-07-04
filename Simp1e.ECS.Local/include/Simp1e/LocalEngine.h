#pragma once

#include <Simp1e/Engine.h>

#include "LocalCommandManager.h"
#include "LocalEntityManager.h"
#include "LocalEventManager.h"
#include "LocalSystemManager.h"

namespace Simp1e {

    class LocalEngine : public Engine<LocalEntityManager, LocalSystemManager, LocalEventManager, LocalCommandManager> {
    public:
        void AddDefaultSystemGroups() {
            GetSystemManagerInstance().AddGroup(DefaultSystemGroupTypes::InitializationGroup);
            GetSystemManagerInstance().AddGroup(DefaultSystemGroupTypes::SimulationGroup);
            GetSystemManagerInstance().AddGroup(DefaultSystemGroupTypes::PresentationGroup);
        }
    };
}
