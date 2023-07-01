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
#include <Simp1e/IEnvironmentManagerService.h>
#include <Simp1e/LabelComponent.h>
#include <Simp1e/ServiceHostClient.h>
#include <Simp1e/SystemPointerManagerClient.h>
#include <Simp1e/TextComponent.h>

#include <memory>

using namespace Simp1e;

constexpr auto* ENVIRONMENT_NAME = "Default";

IEnvironment*                               _environment;
std::unique_ptr<SystemPointerManagerClient> systemManager;

// Experimentation...
class QtGUISystem {
    IEnvironment* _environment;

    void OnLabelCreated(Entity entity, ComponentType componentType) {
        _Log_("QtGUI System says: Label created! - {}", componentType);
    }
    void OnAnythingCreated(Entity entity, ComponentType componentType) {
        _Log_("QtGUI System says: Something created! {}", componentType);
    }

    void UpdateLabel(Entity entity, void* component) {
        _Log_("UpdateLabel({})", entity);

        auto* labelComponent = component_cast<LabelComponent>(component);
        if (!labelComponent) return;

        auto* textComponent = _environment->GetEntityManager()->Get<TextComponent>(entity);
        if (!textComponent) return;

        _Log_("Label for entity {} will have text '{}'", entity, textComponent->GetText());
    }

public:
    DEFINE_SYSTEM_TYPE("QtGUI")

    QtGUISystem(IEnvironment* environment) : _environment(environment) {
        auto* entityEvents = environment->GetEntityManager()->GetEventManager();
        entityEvents->RegisterForAllComponentAdded(this, &QtGUISystem::OnAnythingCreated);
        entityEvents->RegisterForComponentAdded<LabelComponent, QtGUISystem>(this, &QtGUISystem::OnLabelCreated);
    }

    void Update(IEnvironment* environment) {
        environment->GetEntityManager()->ForEach<LabelComponent>(this, &QtGUISystem::UpdateLabel);
    }
};

void SetupSystems(IEnvironment* environment) { systemManager->Add<QtGUISystem>(environment); }

void Initialize(IEnvironment* environment) {
    _environment  = environment;
    systemManager = std::make_unique<SystemPointerManagerClient>(environment->GetSystemManager());
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
    _Log_("Ran update loop");
}
