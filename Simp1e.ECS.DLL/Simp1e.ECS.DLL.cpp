#include <_Log_.h>
_LogToFile_("Simp1e.ECS.log");

#include <Simp1e/Engine.h>
#include <Simp1e/EngineManagerService.h>
#include <Simp1e/EntityPointerManager.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManager.h>

// Default empty structs:
#include <Simp1e/CommandManager.h>
#include <Simp1e/EventManager.h>
#include <Simp1e/SystemManager.h>

using namespace Simp1e;

EngineManagerService                                                             _environmentManagerService;
Engine<EntityPointerManager, SystemPointerManager, EventManager, CommandManager> _environment;

OnSimp1eInit {
    _Log_("Going to construct stuffs...");

    EngineManagerService                                                             _environmentManagerService2;
    Engine<EntityPointerManager, SystemPointerManager, EventManager, CommandManager> _environment2;

    _Log_("Registering ECS Engine Manager service");
    Simp1eServices->RegisterService(&_environmentManagerService);

    _Log_("Adding 'Default' environment");
    _environmentManagerService.GetEngineManager()->RegisterEngine("Default", &_environment);

    _Log_("ECS ready.");
}
