#pragma once

#include <Simp1e/EntityPointerManagerClient.h>

#include <memory>

#include "IECSManagerService.h"

namespace Simp1e {

    class ECSManagerServiceClient {
        std::unique_ptr<EntityPointerManagerClient> _entityPointerManagerClient;

    public:
        ECSManagerServiceClient() = default;
        ECSManagerServiceClient(IEntityManager* entityManager)
            : _entityPointerManagerClient(std::make_unique<EntityPointerManagerClient>(entityManager)) {}

        void SetEntityManager(IEntityManager* entityManager) {
            _entityPointerManagerClient = std::make_unique<EntityPointerManagerClient>(entityManager);
        }

        IEntityManager* GetEntityManager() const { return _entityPointerManagerClient.get(); }
    };
}
