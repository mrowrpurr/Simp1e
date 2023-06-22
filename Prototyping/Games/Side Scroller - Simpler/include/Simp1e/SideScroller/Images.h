#pragma once

#include <Simp1e/Direction.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/CommandTypeMacro.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/MoveCommand.h>
#include <Simp1e/ECS/SetImageCommand.h>
#include <Simp1e/sreal.h>

namespace Simp1e::ECS {

    namespace Images {
        namespace Player {
            namespace Look {
                constexpr auto Right = ":/player/images/look/right.png";
                constexpr auto Left  = ":/player/images/look/left.png";
            }
        }
    }

}
