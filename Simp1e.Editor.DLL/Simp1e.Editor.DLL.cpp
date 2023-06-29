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
    auto* quickTest    = new SomeComponent("Quick Test");
    _ECSManagerService = Simp1eServices->GetService<IECSManagerService>();
    _ECSManager        = _ECSManagerService->GetECSManager();
    _ECS               = _ECSManager->CreateECS(ENVIRONMENT_NAME);
    _ECSManagerServiceClient.SetEntityManager(_ECS->GetEntityManager());
    _EntityManager = _ECSManagerServiceClient.GetEntityManager();
    _Log_("Setup the things, now adding components");
    _EntityManager->AddComponent<SomeComponent>(1, "Hello");
    _EntityManager->AddComponent<SomeComponent>(2, "World");
    _Log_("DONE");
}

OnSimp1eStart {}
