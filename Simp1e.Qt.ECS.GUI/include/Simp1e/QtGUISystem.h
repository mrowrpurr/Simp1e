#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/EntityPointerManagerClient.h>
#include <Simp1e/IEnvironment.h>
#include <Simp1e/IOnClickComponent.h>
#include <Simp1e/IWindowComponent.h>
#include <Simp1e/IWindowMenuComponent.h>
#include <Simp1e/IWindowMenuItemComponent.h>
#include <Simp1e/QActionComponent.h>
#include <Simp1e/QMainWindowComponent.h>
#include <Simp1e/QMenuComponent.h>
#include <_Log_.h>

#include <QMenuBar>
#include <QStatusBar>

namespace Simp1e {

    class QtGUISystem {
        IEnvironment*               _environment;
        EntityPointerManagerClient* _entityManager;

        void OnWindowMenuItemClicked(Entity entity, IWindowMenuItemComponent* windowMenuItemComponent) {
            auto* onClickComponent = _entityManager->Get<IOnClickComponent>(entity);
            if (onClickComponent) {
                auto* functionPtr = onClickComponent->GetFunctionPointer();
                if (functionPtr) functionPtr->Invoke();
                else _Log_("No function pointer found!");
            } else _Log_("No OnClickComponent found!");
        }

        void OnWindowAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowAdded");
            auto* windowComponent = component_cast<IWindowComponent>(component);
            _entityManager->Add<QMainWindowComponent>(entity, windowComponent->GetTitle());
        }

        void OnWindowMenuAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuAdded");
            auto* windowMenuComponent = component_cast<IWindowMenuComponent>(component);
            auto* qMainIWindowComponent =
                _environment->GetEntityManager()->Get<QMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenu = qMainIWindowComponent->GetQMainWindow()->menuBar()->addMenu(windowMenuComponent->GetText());
            _entityManager->Add<QMenuComponent>(entity, qMenu);
        }

        void OnWindowMenuItemAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuItemAdded");
            auto* windowMenuItemComponent = component_cast<IWindowMenuItemComponent>(component);
            auto* windowMenuComponent     = _environment->GetEntityManager()->Get<IWindowMenuComponent>(
                windowMenuItemComponent->GetWindowMenuEntity()
            );
            auto* qMainIWindowComponent =
                _environment->GetEntityManager()->Get<QMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenuComponent =
                _environment->GetEntityManager()->Get<QMenuComponent>(windowMenuItemComponent->GetWindowMenuEntity());
            auto* qAction = qMenuComponent->GetQMenu()->addAction(windowMenuItemComponent->GetText());
            _entityManager->Add<QActionComponent>(entity, qAction);
            QObject::connect(qAction, &QAction::triggered, [this, entity, windowMenuItemComponent]() {
                OnWindowMenuItemClicked(entity, windowMenuItemComponent);
            });
        }

        void UpdateWindow(Entity entity, void* component) {
            auto windowComponent = component_cast<IWindowComponent>(component);
            if (!windowComponent->IsDirtyFlagSet(IWindowComponent::Fields::StatusBarText)) return;
            auto* qMainIWindowComponent = _environment->GetEntityManager()->Get<QMainWindowComponent>(entity);
            qMainIWindowComponent->GetQMainWindow()->statusBar()->showMessage(windowComponent->GetStatusBarText());
            windowComponent->UnsetDirtyFlag(IWindowComponent::Fields::StatusBarText);
        }

    public:
        DEFINE_SYSTEM_TYPE("QtGUI")

        QtGUISystem(IEnvironment* environment) : _environment(environment) {
            _entityManager     = new EntityPointerManagerClient(environment->GetEntityManager());
            auto* entityEvents = environment->GetEntityManager()->GetEventManager();
            entityEvents->RegisterForComponentAdded<IWindowComponent>(this, &QtGUISystem::OnWindowAdded);
            entityEvents->RegisterForComponentAdded<IWindowMenuComponent>(this, &QtGUISystem::OnWindowMenuAdded);
            entityEvents->RegisterForComponentAdded<IWindowMenuItemComponent>(
                this, &QtGUISystem::OnWindowMenuItemAdded
            );
        }

        void Update(IEnvironment* environment, double deltaTime) {
            _Log_("QtGUI Update deltaTime:{}", deltaTime);

            // Let's try updating the status bar text...
            environment->GetEntityManager()->ForEach<IWindowComponent>(this, &QtGUISystem::UpdateWindow);
        }
    };
}
