#pragma once

#include "IECS.h"

namespace Simp1e {

    template <typename EntityManagerT>
    class ECS : public IECS {
        EntityManagerT _entityManager;

    public:
        IEntityManager* GetEntityManager() override { return &_entityManager; }
    };
}
