#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/ComponentCast.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/OnClickComponent.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>
#include <Simp1e/TextComponent.h>
#include <Simp1e/WindowComponent.h>
#include <Simp1e/WindowMenuComponent.h>
#include <Simp1e/WindowMenuItemComponent.h>
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
    auto window = entityManager->CreateEntity();
    entityManager->Add<WindowComponent>(window, "Simp1e Editor");

    auto fileMenu = entityManager->CreateEntity();
    entityManager->Add<WindowMenuComponent>(fileMenu, window, "File");

    auto quitItem = entityManager->CreateEntity();
    entityManager->Add<WindowMenuItemComponent>(quitItem, fileMenu, "Quit");
    entityManager->Add<OnClickComponent>(quitItem, function_pointer([]() {
                                             // TODO ask the Qt application to close. For now, just exit the process:
                                             exit(0);
                                         }));

    auto helpMenu = entityManager->CreateEntity();
    entityManager->Add<WindowMenuComponent>(helpMenu, window, "Help");

    auto aboutItem = entityManager->CreateEntity();
    entityManager->Add<WindowMenuItemComponent>(aboutItem, helpMenu, "About");
    entityManager->Add<OnClickComponent>(aboutItem, function_pointer([]() {
                                             // TODO show a dialog with information about the application
                                             _Log_("TODO: show a dialog with information about the application");
                                         }));
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
