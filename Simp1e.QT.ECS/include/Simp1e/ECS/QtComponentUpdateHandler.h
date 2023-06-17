#pragma once

#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/ComponentPointer.h>
#include <Simp1e/ECS/Entity.h>
#include <Simp1e/ECS/Game.h>


namespace Simp1e::ECS {

    struct QtComponentUpdateHandler {
        virtual void Update(Game& game, Entity entity, ComponentPointer& component) = 0;
    };
}
