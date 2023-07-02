#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/ApplicationComponent.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/TextComponent.h>
#include <Simp1e/WindowComponent.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Default";

std::unique_ptr<EntityPointerManagerClient> entityManager;

void CreateEntities() {
    _Log_("Creating entities");
    //

    auto windowEntity = entityManager->CreateEntity();
    entityManager->Add<WindowComponent>(windowEntity, "Simp1e Editor");
}

void Initialize(IEnvironment* environment) {
    entityManager = std::make_unique<EntityPointerManagerClient>(environment->GetEntityManager());
}

OnSimp1eLoad {
    _Log_("Load");
    if (auto* environmentManagerService = Simp1eServices->GetService<IEnvironmentManagerService>())
        if (auto* environmentManager = environmentManagerService->GetEnvironmentManager())
            if (auto* environment = environmentManager->GetEnvironment(ENVIRONMENT_NAME)) Initialize(environment);
    _Log_("Loaded");
}

OnSimp1eStart { CreateEntities(); }
