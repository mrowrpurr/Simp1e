#include <_Log_.h>
_LogToFile_("Simp1e.Editor2.log");

#include <Simp1e/IEngineManagerService.h>
#include <Simp1e/ServiceHostClient.h>

constexpr auto* ENVIRONMENT_NAME = "Simp1e.Editor";

Simp1e::IEngine* environment = nullptr;

OnSimp1eLoad {
    _Log_("Simp1e.Editor2 loaded");
    if (auto* service = Simp1eServices->GetService<Simp1e::IEngineManagerService>())
        environment = service->GetEngineManager()->CreateEngine(ENVIRONMENT_NAME);
    else _Log_("Failed to get EngineManagerService");
}

OnSimp1eStart {
    // Qt stuffs! Should really be from JSON tho. Loaded components from filesystem.
    //
    // But first let's just render a window using App components.
}
