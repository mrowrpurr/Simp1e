#include <_Log_.h>
_LogToFile_("Simp1e.ECS.log");

#include <Simp1e/ECSManagerService.h>
#include <Simp1e/EntityPointerManager.h>
#include <Simp1e/ServiceHostClient.h>

using namespace Simp1e;

ECSManagerService<EntityPointerManager> _ECSManagerService;

OnSimp1eInit { _Log_("Init."); }
OnSimp1eLoad {
    _Log_("Registering ECS EnvironmentManager");
    Simp1eServices->RegisterService(&_ECSManagerService);
}
