#pragma once

#include <Simp1e/EntityPointerManager.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <_Log_.h>

#include <memory>

#include "IECSManagerService.h"


namespace Simp1e {

    class ECSManagerServiceClient {
        std::unique_ptr<EntityPointerManagerClient> _entityPointerManagerClient;

    public:
        void SetEntityManager(EntityPointerManager* entityManager) {
            _entityPointerManagerClient = std::make_unique<EntityPointerManagerClient>(entityManager);
        }
        void SetEntityManager(IEntityManager* entityManager) {
            auto* entityPointerManager = dynamic_cast<EntityPointerManager*>(entityManager);
            if (!entityPointerManager) {
                _Log_("[ECSManagerServiceClient] SetEntityManager: entityManager is not EntityPointerManager");
                return;
            }
            _entityPointerManagerClient = std::make_unique<EntityPointerManagerClient>(entityPointerManager);
        }

        EntityPointerManagerClient* GetEntityManager() const { return _entityPointerManagerClient.get(); }
    };
}
