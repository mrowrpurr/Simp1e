#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/ICanvasComponent.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/ILabelComponent.h>
#include <Simp1e/IOnClickComponent.h>
#include <Simp1e/IWindowComponent.h>
#include <Simp1e/IWindowMenuComponent.h>
#include <Simp1e/IWindowMenuItemComponent.h>
#include <Simp1e/QActionComponent.h>
#include <Simp1e/QMainWindowComponent.h>
#include <Simp1e/QMenuComponent.h>
#include <Simp1e/QSimp1eGraphicsScene.h>
#include <Simp1e/QSimp1eGraphicsView.h>
#include <Simp1e/QWidgetComponent.h>
#include <_Log_.h>

#include <QLabel>
#include <QMenuBar>
#include <QStatusBar>

namespace Simp1e {

    class QtGuiSystem {
        IEngine* _engine;

        IEntityManager* entityManager() const { return _engine->GetEntityManager(); }

        QWidget* GetParentWidget(Entity parentEntity) {
            auto* qWidgetComponent = entityManager()->Get<QWidgetComponent>(parentEntity);
            if (!qWidgetComponent) return nullptr;
            return qWidgetComponent->GetQWidget();
        }

        QLayout* GetParentLayout(Entity parentEntity) {
            if (auto* parentWidget = GetParentWidget(parentEntity)) return parentWidget->layout();
            return nullptr;
        }

        void OnWindowMenuItemClicked(Entity entity, IWindowMenuItemComponent* windowMenuItemComponent) {
            auto* clickFunction = windowMenuItemComponent->GetClickFunction();
            if (clickFunction) clickFunction->Invoke();
        }

        void OnWindowAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowAdded");
            auto* windowComponent = component_cast<IWindowComponent>(component);
            auto* qMainWindowComponent =
                entityManager()->Add<QMainWindowComponent>(entity, windowComponent->GetTitle());
            entityManager()->Add<QWidgetComponent>(entity, qMainWindowComponent->GetCentralQWidget());
        }

        void OnWindowMenuAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuAdded");
            auto* windowMenuComponent = component_cast<IWindowMenuComponent>(component);
            auto* qMainIWindowComponent =
                _engine->GetEntityManager()->Get<QMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenu = qMainIWindowComponent->GetQMainWindow()->menuBar()->addMenu(windowMenuComponent->GetText());
            entityManager()->Add<QMenuComponent>(entity, qMenu);
        }

        void OnWindowMenuItemAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuItemAdded");
            auto* windowMenuItemComponent = component_cast<IWindowMenuItemComponent>(component);
            auto* windowMenuComponent =
                _engine->GetEntityManager()->Get<IWindowMenuComponent>(windowMenuItemComponent->GetWindowMenuEntity());
            auto* qMainIWindowComponent =
                _engine->GetEntityManager()->Get<QMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenuComponent =
                _engine->GetEntityManager()->Get<QMenuComponent>(windowMenuItemComponent->GetWindowMenuEntity());
            auto* qAction = qMenuComponent->GetQMenu()->addAction(windowMenuItemComponent->GetText());
            entityManager()->Add<QActionComponent>(entity, qAction);
            QObject::connect(qAction, &QAction::triggered, [this, entity, windowMenuItemComponent]() {
                OnWindowMenuItemClicked(entity, windowMenuItemComponent);
            });
        }

        void OnLabelAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> LabelAdded");
            auto* labelComponent = component_cast<ILabelComponent>(component);
            if (auto* layout = GetParentLayout(labelComponent->GetParentEntity())) {
                auto* qLabel = new QLabel();
                qLabel->setText(labelComponent->GetText());
                layout->addWidget(qLabel);
            }
        }

        void OnCanvasAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> CanvasAdded");
            auto* canvasComponent = component_cast<ICanvasComponent>(component);
            if (auto* layout = GetParentLayout(canvasComponent->GetParentEntity())) {
                auto* view  = new QSimp1eGraphicsView();
                auto* scene = new QSimp1eGraphicsScene();
                view->setScene(scene);
                layout->addWidget(view);
            }
        }

        void UpdateWindow(Entity entity, void* component) {
            auto windowComponent = component_cast<IWindowComponent>(component);
            if (!windowComponent->IsDirtyFlagSet(IWindowComponent::Fields::StatusBarText)) return;
            auto* qMainIWindowComponent = _engine->GetEntityManager()->Get<QMainWindowComponent>(entity);
            qMainIWindowComponent->GetQMainWindow()->statusBar()->showMessage(windowComponent->GetStatusBarText());
            windowComponent->UnsetDirtyFlag(IWindowComponent::Fields::StatusBarText);
        }

    public:
        DEFINE_SYSTEM_TYPE("QtGUI")

        QtGuiSystem(IEngine* engine) : _engine(engine) {
            auto* entityEvents = entityManager()->GetEventManager();
            entityEvents->RegisterForComponentAdded<IWindowComponent>(this, &QtGuiSystem::OnWindowAdded);
            entityEvents->RegisterForComponentAdded<IWindowMenuComponent>(this, &QtGuiSystem::OnWindowMenuAdded);
            entityEvents->RegisterForComponentAdded<IWindowMenuItemComponent>(
                this, &QtGuiSystem::OnWindowMenuItemAdded
            );
            entityEvents->RegisterForComponentAdded<ILabelComponent>(this, &QtGuiSystem::OnLabelAdded);
            entityEvents->RegisterForComponentAdded<ICanvasComponent>(this, &QtGuiSystem::OnCanvasAdded);
        }

        void Update(IEngine* engine, double deltaTime) {
            engine->GetEntityManager()->ForEach<IWindowComponent>(
                function_pointer(this, &QtGuiSystem::UpdateWindow).get()
            );
        }
    };
}
