#include <_Log_.h>
_LogToFile_("Simp1e.ECS.log");

#include <Simp1e/EnvironmentManagerService.h>
#include <Simp1e/ServiceHostClient.h>

Simp1e::EnvironmentManagerService environmentManagerService;

OnSimp1eInit { _Log_("Init."); }
OnSimp1eLoad {
    _Log_("Registering ECS EnvironmentManager");
    Simp1eServices->RegisterService<Simp1e::EnvironmentManagerService>(&environmentManagerService);
}
