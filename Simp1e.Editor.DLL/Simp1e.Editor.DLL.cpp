#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/ECSManagerServiceClient.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/ServiceHostClient.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Simp1e.Editor";

IECS*                   _ECS;
IECSManager*            _ECSManager;
IECSManagerService*     _ECSManagerService;
ECSManagerServiceClient _ECSManagerServiceClient;

OnSimp1eLoad {
    _Log_("Hi from Editor Load()");
    _ECSManagerService = Simp1eServices->GetService<IECSManagerService>();
    _ECSManager        = _ECSManagerService->GetECSManager();
    _ECS               = _ECSManager->CreateECS(ENVIRONMENT_NAME);
    _ECSManagerServiceClient.SetEntityManager(_ECS->GetEntityManager());
    _Log_("All the things ran");
}

OnSimp1eStart {}
