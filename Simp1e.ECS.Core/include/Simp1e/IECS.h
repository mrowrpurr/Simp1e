#pragma once

#include "IEntityManager.h"

namespace Simp1e {

    struct IECS {
        virtual ~IECS() = default;

        virtual IEntityManager* GetEntityManager() = 0;
    };
}
