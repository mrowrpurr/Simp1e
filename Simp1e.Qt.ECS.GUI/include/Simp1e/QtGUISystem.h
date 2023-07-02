#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironment.h>
#include <Simp1e/QMainWindowComponent.h>
#include <Simp1e/WindowComponent.h>

#include <QStatusBar>

namespace Simp1e {

    class QtGUISystem {
        IEnvironment*               _environment;
        EntityPointerManagerClient* _entityManager;

        void OnWindowAdded(Entity entity, ComponentType componentType, void* component) {
            auto* windowComponent = component_cast<WindowComponent>(component);
            _entityManager->Add<QMainWindowComponent>(entity, windowComponent->GetTitle());
        }

        void UpdateWindow(Entity entity, void* component) {
            auto windowComponent = component_cast<WindowComponent>(component);
            if (!windowComponent->IsDirtyFlagSet(WindowComponent::Fields::StatusBarText)) return;
            auto* qMainWindowComponent = _environment->GetEntityManager()->Get<QMainWindowComponent>(entity);
            qMainWindowComponent->GetQMainWindow()->statusBar()->showMessage(windowComponent->GetStatusBarText());
            windowComponent->RemoveDirtyFlag(WindowComponent::Fields::StatusBarText);
        }

    public:
        DEFINE_SYSTEM_TYPE("QtGUI")

        QtGUISystem(IEnvironment* environment) : _environment(environment) {
            _entityManager     = new EntityPointerManagerClient(environment->GetEntityManager());
            auto* entityEvents = environment->GetEntityManager()->GetEventManager();
            entityEvents->RegisterForComponentAdded<WindowComponent>(this, &QtGUISystem::OnWindowAdded);
        }

        void Update(IEnvironment* environment, double deltaTime) {
            _Log_("QtGUI Update deltaTime:{}", deltaTime);

            // Let's try updating the status bar text...
            environment->GetEntityManager()->ForEach<WindowComponent>(this, &QtGUISystem::UpdateWindow);
        }
    };
}
