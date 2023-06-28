#include <_Log_.h>
_LogToFile_("Simp1e.Editor.log");

#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/ServiceHostClient.h>

constexpr auto* ENVIRONMENT_NAME = "Simp1e.Editor";

OnSimp1eInit { _Log_("Simp1eInit called"); }
OnSimp1eLoad { _Log_("Simp1eLoad called"); }
OnSimp1eStart {
    _Log_("Simp1eStart called");
    _Log_("Let's interact with the shared ECS services...");
    auto* service = Simp1eServices->GetService<Simp1e::IEnvironmentManagerService>();
    if (!service) {
        _Log_("Failed to get IEnvironmentManagerService");
        return;
    } else {
        _Log_("Got IEnvironmentManagerService");
        auto* environmentManager = service->GetEnvironmentManager();
        auto* environment        = environmentManager->GetEnvironment(ENVIRONMENT_NAME);
        if (!environment) environment = environmentManager->CreateEnvironment(ENVIRONMENT_NAME);
        if (environment) _Log_("Got environment");
        else _Log_("Failed to get environment");
    }
}

OnSimp1eUnload { _Log_("Simp1eUnload called"); }
