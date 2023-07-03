#include <_Log_.h>
_LogToFile_("Simp1e.ECS.log");

#include <Simp1e/EntityPointerManager.h>
#include <Simp1e/Environment.h>
#include <Simp1e/EnvironmentManagerService.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManager.h>

// Default empty structs:
#include <Simp1e/CommandManager.h>
#include <Simp1e/EventManager.h>
#include <Simp1e/SystemManager.h>

using namespace Simp1e;

EnvironmentManagerService                                                             _environmentManagerService;
Environment<EntityPointerManager, SystemPointerManager, EventManager, CommandManager> _environment;

OnSimp1eInit {
    _Log_("Going to construct stuffs...");

    EnvironmentManagerService                                                             _environmentManagerService2;
    Environment<EntityPointerManager, SystemPointerManager, EventManager, CommandManager> _environment2;

    _Log_("Registering ECS Environment Manager service");
    Simp1eServices->RegisterService(&_environmentManagerService);

    _Log_("Adding 'Default' environment");
    _environmentManagerService.GetEnvironmentManager()->RegisterEnvironment("Default", &_environment);

    _Log_("ECS ready.");
}
