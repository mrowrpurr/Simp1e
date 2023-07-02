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

#include <Simp1e/ComponentCast.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/QMainWindowComponent.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>
#include <Simp1e/TextComponent.h>
#include <Simp1e/WindowComponent.h>

#include <QApplication>
#include <memory>

using namespace Simp1e;

int          argc = 0;
QApplication app{argc, nullptr};

constexpr auto* ENVIRONMENT_NAME = "Default";

IEnvironment*                               _environment;
std::unique_ptr<SystemPointerManagerClient> systemManager;
std::unique_ptr<EntityPointerManagerClient> entityManager;

// Experimentation...
class QtGUISystem {
    IEnvironment* _environment;

    void OnWindowAdded(Entity entity, ComponentType componentType, void* component) {
        auto* windowComponent = component_cast<WindowComponent>(component);
        entityManager->Add<QMainWindowComponent>(entity, windowComponent->GetTitle());
    }

public:
    DEFINE_SYSTEM_TYPE("QtGUI")

    QtGUISystem(IEnvironment* environment) : _environment(environment) {
        auto* entityEvents = environment->GetEntityManager()->GetEventManager();
        entityEvents->RegisterForComponentAdded<WindowComponent>(this, &QtGUISystem::OnWindowAdded);
    }

    void Update(IEnvironment* environment) {
        _Log_("[Update] Getting all LabelComponent...");
        // environment->GetEntityManager()->ForEach<LabelComponent>(this, &QtGUISystem::UpdateLabel);
    }
};

void SetupSystems(IEnvironment* environment) { systemManager->Add<QtGUISystem>(environment); }

void Initialize(IEnvironment* environment) {
    _environment  = environment;
    systemManager = std::make_unique<SystemPointerManagerClient>(environment->GetSystemManager());
    entityManager = std::make_unique<EntityPointerManagerClient>(environment->GetEntityManager());
    SetupSystems(environment);
}

OnSimp1eLoad {
    _Log_("Init");
    if (auto* environmentManagerService = Simp1eServices->GetService<IEnvironmentManagerService>())
        if (auto* environmentManager = environmentManagerService->GetEnvironmentManager())
            if (auto* environment = environmentManager->GetEnvironment(ENVIRONMENT_NAME)) Initialize(environment);
}

OnSimp1eStart {
    _Log_("Qt ECS GUI start");
    _Log_("Running 1 update loop");
    _environment->GetSystemManager()->Update(_environment);
    // _Log_("Ran update loop");

    _Log_("Run Qt application");
    app.setStyle("Fusion");
    app.exec();
}
