#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/ECSManagerServiceClient.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/ServiceHostClient.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Simp1e.Editor";

struct SomeComponent {
    const char*        text;
    static const char* GetComponentType() { return "SomeComponent"; }
    SomeComponent(const char* text) : text(text) {}
};

IECS*                   _ECS;
IECSManager*            _ECSManager;
IECSManagerService*     _ECSManagerService;
ECSManagerServiceClient _ECSManagerServiceClient;
//
EntityPointerManagerClient* _EntityManager;

OnSimp1eLoad {
    _Log_("Hi from Editor Load()");
    auto* quickTest                      = new SomeComponent("Quick Test");
    _ECSManagerService                   = Simp1eServices->GetService<IECSManagerService>();
    _ECSManager                          = _ECSManagerService->GetECSManager();
    _ECS                                 = _ECSManager->CreateECS(ENVIRONMENT_NAME);
    IEntityManager* serviceEntityManager = _ECS->GetEntityManager();
    _ECSManagerServiceClient.SetEntityManager(serviceEntityManager);
    _EntityManager = _ECSManagerServiceClient.GetEntityManager();

    _Log_("Setup the things, now adding centities");
    auto entity1 = _EntityManager->CreateEntity();
    auto entity2 = _EntityManager->CreateEntity();

    _Log_("Adding components");
    _EntityManager->AddComponent<SomeComponent>(entity1, "Hello");
    _EntityManager->AddComponent<SomeComponent>(entity2, "World");
    _Log_("??????????? Let's read from the interface/pointer storage");

    serviceEntityManager->ForEachComponent(SomeComponent::GetComponentType(), [](Entity, void*) {
        _Log_("ForEachComponent<SomeComponent> callback");
    });
    _Log_("DONE");
}

OnSimp1eStart {}
