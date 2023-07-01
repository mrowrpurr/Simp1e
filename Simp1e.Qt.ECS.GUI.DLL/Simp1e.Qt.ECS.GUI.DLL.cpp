// #include <QApplication>
// #include <QWidget>

// Let's have something that listens for ToolbarButton component creation
// and adds them to out window.

// Or just Label first...

// OnSimp1eStart {
//     int          argc;
//     QApplication a(argc, nullptr);
//     QWidget      w;
//     w.setWindowTitle("Hello World!");
//     w.show();
//     a.exec();
// }

#include <_Log_.h>
_LogToFile_("Simp1e.Qt.ECS.GUI.log");

#include <Simp1e/DefineSystemType.h>
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Default";

std::unique_ptr<SystemPointerManagerClient> systemManager;

// Experimentation...
class QtGUISystem {
    void OnLabelCreated(Entity entity, ComponentType componentType) {
        _Log_("QtGUI System says: Label created! - {}", componentType);
    }
    void OnAnythingCreated(Entity entity, ComponentType componentType) {
        _Log_("QtGUI System says: Something created! {}", componentType);
    }

public:
    DEFINE_SYSTEM_TYPE("QtGUI")

    QtGUISystem(IEnvironment* environment) {
        auto* entityEvents = environment->GetEntityManager()->GetEventManager();
        // entityEvents->OnComponentAdded(&QtGUISystem::OnAnythingCreated);
    }

    void Update(IEnvironment*) {}
};

void SetupSystems(IEnvironment* environment) { systemManager->Add<QtGUISystem>(environment); }

void Initialize(IEnvironment* environment) {
    systemManager = std::make_unique<SystemPointerManagerClient>(environment->GetSystemManager());
    SetupSystems(environment);
}

OnSimp1eLoad {
    _Log_("Init");
    if (auto* environmentManagerService = Simp1eServices->GetService<IEnvironmentManagerService>())
        if (auto* environmentManager = environmentManagerService->GetEnvironmentManager())
            if (auto* environment = environmentManager->GetEnvironment(ENVIRONMENT_NAME)) Initialize(environment);
}

OnSimp1eStart {}
