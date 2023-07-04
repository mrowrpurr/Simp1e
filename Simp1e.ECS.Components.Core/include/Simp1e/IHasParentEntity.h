#pragma once

#include <Simp1e/Entity.h>

namespace Simp1e {

    struct IHasParentEntity {
        virtual Entity GetParentEntity() const              = 0;
        virtual void   SetParentEntity(Entity parentEntity) = 0;
    };
}