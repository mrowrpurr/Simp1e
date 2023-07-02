#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/ComponentCast.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/LabelComponent.h>
// #include <Simp1e/MenuComponent.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>
#include <Simp1e/TextComponent.h>
#include <Simp1e/WindowComponent.h>
#include <string_format.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Default";

std::unique_ptr<EntityPointerManagerClient> entityManager;
std::unique_ptr<SystemPointerManagerClient> systemManager;

#include <Simp1e/DefineSystemType.h>
class UpdateWindowStatusBarSystem {
    double _totalTime;

    void UpdateWindow(Entity entity, void* component) {
        auto* windowComponent = component_cast<IWindowComponent>(component);
        windowComponent->SetStatusBarText(string_format("Time: {}", _totalTime).c_str());
    }

public:
    DEFINE_SYSTEM_TYPE("UpdateWindowStatusBar");

    void Update(IEnvironment* environment, double deltaTime) {
        _totalTime += deltaTime;
        environment->GetEntityManager()->ForEach<IWindowComponent>(this, &UpdateWindowStatusBarSystem::UpdateWindow);
    }
};

void CreateEntities() {
    auto windowEntity = entityManager->CreateEntity();
    entityManager->Add<WindowComponent>(windowEntity, "Simp1e Editor");

    // Menu items

    // Toolbar items
}

void Initialize(IEnvironment* environment) {
    entityManager = std::make_unique<EntityPointerManagerClient>(environment->GetEntityManager());
    systemManager = std::make_unique<SystemPointerManagerClient>(environment->GetSystemManager());
    systemManager->Add<UpdateWindowStatusBarSystem>();
}

OnSimp1eLoad {
    _Log_("Load");
    if (auto* environmentManagerService = Simp1eServices->GetService<IEnvironmentManagerService>())
        if (auto* environmentManager = environmentManagerService->GetEnvironmentManager())
            if (auto* environment = environmentManager->GetEnvironment(ENVIRONMENT_NAME)) Initialize(environment);
    _Log_("Loaded");
}

OnSimp1eStart { CreateEntities(); }
