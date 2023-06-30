#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Default";

struct SomeComponent {
    const char*          text;
    static ComponentType GetComponentType() { return "SomeComponent"; }
    SomeComponent(const char* text) : text(text) {}
};

struct SomeSystem {
    static SystemType GetSystemType() { return "SomeSystem"; }
    void              Update() { _Log_("SomeSystem update!"); }  // TODO update this so it gets an IEntityManager*
};

OnSimp1eLoad {
    _Log_("BEGIN LOAD");
    if (auto* environmentManagerService = Simp1eServices->GetService<IEnvironmentManagerService>()) {
        if (auto* environmentManager = environmentManagerService->GetEnvironmentManager()) {
            if (auto* environment = environmentManager->GetEnvironment(ENVIRONMENT_NAME)) {
                EntityPointerManagerClient entityManager(environment->GetEntityManager());

                auto entity1 = entityManager.CreateEntity();
                entityManager.AddComponent<SomeComponent>(entity1, "Hello, World");

                auto entity2 = entityManager.CreateEntity();
                entityManager.AddComponent<SomeComponent>(entity2, "Goodnight, Moon");

                entityManager.ForEachComponent<SomeComponent>([](Entity entity, void* component) {
                    auto* someComponent = static_cast<SomeComponent*>(component);
                    _Log_("Component text: {}", someComponent->text);
                });

                SystemPointerManagerClient systemManager(environment->GetSystemManager());
                systemManager.AddSystem<SomeSystem>();
                systemManager.Update();
            }
        }
    }
    _Log_("END LOAD");
}

OnSimp1eStart {}
