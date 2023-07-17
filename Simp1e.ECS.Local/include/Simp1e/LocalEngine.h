#pragma once

#include <Simp1e/DefaultSystemGroupTypes.h>
#include <Simp1e/Engine.h>

#include "LocalCommandManager.h"
#include "LocalEntityManager.h"
#include "LocalEventManager.h"
#include "LocalInputManager.h"
#include "LocalSystemGroupManager.h"

namespace Simp1e {

    class LocalEngine
        : public Engine<
              LocalEntityManager, LocalSystemGroupManager, LocalEventManager, LocalCommandManager, LocalInputManager> {
    public:
        void AddDefaultSystemGroups() {
            GetSystemGroups()->AddGroup(DefaultSystemGroupTypes::InitializationGroup);
            GetSystemGroups()->AddGroup(DefaultSystemGroupTypes::SimulationGroup);
            GetSystemGroups()->AddGroup(DefaultSystemGroupTypes::PresentationGroup);
        }
    };
}
