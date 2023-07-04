#pragma once

#include "SystemGroupType.h"

namespace Simp1e::DefaultSystemGroupTypes {
    constexpr SystemGroupType FirstGroup          = "Default:First";
    constexpr SystemGroupType InitializationGroup = "Default:Initialization";
    constexpr SystemGroupType SimulationGroup     = "Default:Simulation";
    constexpr SystemGroupType PresentationGroup   = "Default:Presentation";
    constexpr SystemGroupType LastGroup           = "Default:Last";
}
