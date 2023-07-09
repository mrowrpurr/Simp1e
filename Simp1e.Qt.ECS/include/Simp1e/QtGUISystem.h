#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeFromType.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/ICanvasComponent.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IFillColorComponent.h>
#include <Simp1e/IImageComponent.h>
#include <Simp1e/ILabelComponent.h>
#include <Simp1e/IOnClickComponent.h>
#include <Simp1e/IPositionComponent.h>
#include <Simp1e/IRectangleComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/IWindowComponent.h>
#include <Simp1e/IWindowMenuComponent.h>
#include <Simp1e/IWindowMenuItemComponent.h>
#include <Simp1e/QActionComponent.h>
#include <Simp1e/QMainWindowComponent.h>
#include <Simp1e/QMenuComponent.h>
#include <Simp1e/QSimp1eGraphicsItem.h>
#include <Simp1e/QSimp1eGraphicsItemComponent.h>
#include <Simp1e/QSimp1eGraphicsScene.h>
#include <Simp1e/QSimp1eGraphicsView.h>
#include <Simp1e/QWidgetComponent.h>
#include <Simp1e/ToQRect.h>
#include <Simp1e/ToQSize.h>
#include <_Log_.h>

#include <QLabel>
#include <QMenuBar>
#include <QScrollBar>
#include <QStatusBar>
#include <memory>
#include <unordered_map>
#include <vector>

#include "IQtComponentPainter.h"
#include "IQtComponentUpdateHandler.h"
#include "QSimp1eImageComponent.h"
#include "QtImageComponentPainter.h"
#include "QtPositionComponentUpdateHandler.h"
#include "QtRectangleComponentPainter.h"
#include "QtSizeComponentUpdateHandler.h"

namespace Simp1e {

    class QtGuiSystem {
        IEngine* _engine;
        IFunctionPointer<void(Entity, QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>*
            _entityPaintFunctionPointer;

        std::unordered_map<ComponentTypeHashKey, std::unique_ptr<IQtComponentPainter>> _componentPainters;
        std::vector<ComponentTypeHashKey>                                              _componentPaintersOrder;

        std::unordered_map<ComponentTypeHashKey, std::unique_ptr<IQtComponentUpdateHandler>> _componentUpdateHandlers;

        ///////
        QSimp1eGraphicsScene* _canvasScene;

        IEntityManager* entityManager() const { return _engine->GetEntities(); }

        QWidget* GetParentWidget(Entity parentEntity) {
            auto* qWidgetComponent = entityManager()->GetComponent<QWidgetComponent>(parentEntity);
            if (!qWidgetComponent) return nullptr;
            return qWidgetComponent->GetQWidget();
        }

        QLayout* GetParentLayout(Entity parentEntity) {
            if (auto* parentWidget = GetParentWidget(parentEntity)) return parentWidget->layout();
            return nullptr;
        }

        void OnWindowMenuItemClicked(Entity entity, IWindowMenuItemComponent* windowMenuItemComponent) {
            auto* clickFunction = windowMenuItemComponent->GetClickFunction();
            if (clickFunction) clickFunction->invoke();
        }

        void OnWindowAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowAdded");
            auto* windowComponent = component_cast<IWindowComponent>(component);
            auto* qMainWindowComponent =
                entityManager()->AddComponent<QMainWindowComponent>(entity, windowComponent->GetTitle());
            entityManager()->AddComponent<QWidgetComponent>(entity, qMainWindowComponent->GetCentralQWidget());
        }

        void OnWindowMenuAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuAdded");
            auto* windowMenuComponent = component_cast<IWindowMenuComponent>(component);
            auto* qMainIWindowComponent =
                _engine->GetEntities()->GetComponent<QMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenu = qMainIWindowComponent->GetQMainWindow()->menuBar()->addMenu(windowMenuComponent->GetText());
            entityManager()->AddComponent<QMenuComponent>(entity, qMenu);
        }

        void OnWindowMenuItemAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuItemAdded");
            auto* windowMenuItemComponent = component_cast<IWindowMenuItemComponent>(component);
            auto* windowMenuComponent     = _engine->GetEntities()->GetComponent<IWindowMenuComponent>(
                windowMenuItemComponent->GetWindowMenuEntity()
            );
            auto* qMainIWindowComponent =
                _engine->GetEntities()->GetComponent<QMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenuComponent =
                _engine->GetEntities()->GetComponent<QMenuComponent>(windowMenuItemComponent->GetWindowMenuEntity());
            auto* qAction = qMenuComponent->GetQMenu()->addAction(windowMenuItemComponent->GetText());
            entityManager()->AddComponent<QActionComponent>(entity, qAction);
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
                auto* testTempLabel = new QLabel{"CANVAS IS HERE"};
                layout->addWidget(testTempLabel);
                auto* view  = new QSimp1eGraphicsView();
                auto* scene = new QSimp1eGraphicsScene();
                auto  size  = canvasComponent->GetSize();
                if (!size.IsNull()) scene->setSceneRect(ToQRect(size));
                view->setScene(scene);
                view->move(0, 0);
                view->horizontalScrollBar()->setValue(0);
                layout->addWidget(view);
                view->FitSceneToViewHeight();
                // Save for graphical components to render on:
                _canvasScene = scene;
                // Gross hack, just for now:
                layout->parentWidget()->parentWidget()->resize(ToQSize(size));
            }
        }

        void OnPositionAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> PositionAdded");
            auto* positionComponent = component_cast<IPositionComponent>(component);
            positionComponent->SetDirty(true);  // Start off in need of an update.
        }

        void OnSizeAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> SizeAdded");
            auto* sizeComponent = component_cast<ISizeComponent>(component);
            sizeComponent->SetDirty(true);  // Start off in need of an update.
        }

        void OnRectangleAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> RectangleAdded");
            if (!_canvasScene) return;
            auto* rectangleComponent = component_cast<IRectangleComponent>(component);
            auto* graphicsItem       = entityManager()->AddComponent<QSimp1eGraphicsItemComponent>(
                entity, entity, _entityPaintFunctionPointer
            );
            _Log_("Adding a rectangle graphics item to the scene");
            _canvasScene->addItem(graphicsItem->GetQSimp1eGraphicsItem());
        }

        void OnImageAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> ImageAdded");
            if (!_canvasScene) return;
            auto* imageComponent = component_cast<IImageComponent>(component);
            auto* qImageComponent =
                entityManager()->AddComponent<QSimp1eImageComponent>(entity, imageComponent->GetImagePath());
        }

        void UpdateWindow(Entity entity, void* component) {
            auto windowComponent = component_cast<IWindowComponent>(component);
            if (!windowComponent->IsDirtyFlagSet(IWindowComponent::Fields::StatusBarText)) return;
            auto* qMainIWindowComponent = _engine->GetEntities()->GetComponent<QMainWindowComponent>(entity);
            qMainIWindowComponent->GetQMainWindow()->statusBar()->showMessage(windowComponent->GetStatusBarText());
            windowComponent->UnsetDirtyFlag(IWindowComponent::Fields::StatusBarText);
        }

        void OnPaintGraphicsItem(
            Entity entity, QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget = nullptr
        ) {
            for (int i = 0; i < _componentPaintersOrder.size(); ++i) {
                auto  componentTypeHashKey = _componentPaintersOrder[i];
                auto& componentPainter     = _componentPainters[componentTypeHashKey];
                auto* component =
                    entityManager()->GetComponentPointer(entity, ComponentTypeFromHashKey(componentTypeHashKey));
                componentPainter->Paint(_engine, entity, component, painter, option, widget);
            }
        }

    public:
        DEFINE_SYSTEM_TYPE("QtGUI")

        QtGuiSystem(IEngine* engine) : _engine(engine) {
            _entityPaintFunctionPointer = new_function_pointer(this, &QtGuiSystem::OnPaintGraphicsItem);
            auto* entityEvents          = entityManager()->GetEventManager();
            entityEvents->RegisterForComponentAdded<IWindowComponent>({this, &QtGuiSystem::OnWindowAdded});
            entityEvents->RegisterForComponentAdded<IWindowMenuComponent>({this, &QtGuiSystem::OnWindowMenuAdded});
            entityEvents->RegisterForComponentAdded<IWindowMenuItemComponent>(
                {this, &QtGuiSystem::OnWindowMenuItemAdded}
            );
            entityEvents->RegisterForComponentAdded<ILabelComponent>({this, &QtGuiSystem::OnLabelAdded});
            entityEvents->RegisterForComponentAdded<ICanvasComponent>({this, &QtGuiSystem::OnCanvasAdded});
            entityEvents->RegisterForComponentAdded<IRectangleComponent>({this, &QtGuiSystem::OnRectangleAdded});
            entityEvents->RegisterForComponentAdded<IPositionComponent>({this, &QtGuiSystem::OnPositionAdded});
            entityEvents->RegisterForComponentAdded<ISizeComponent>({this, &QtGuiSystem::OnSizeAdded});
            entityEvents->RegisterForComponentAdded<IImageComponent>({this, &QtGuiSystem::OnImageAdded});
            // RegisterComponentPainter<IRectangleComponent, QtRectangleComponentPainter>();
            RegisterComponentPainter<IImageComponent, QtImageComponentPainter>();
            RegisterComponentUpdateHandler<IPositionComponent, QtPositionComponentUpdateHandler>();
            RegisterComponentUpdateHandler<ISizeComponent, QtSizeComponentUpdateHandler>();
        }

        void RegisterComponentPainter(ComponentTypeHashKey componentTypeHashKey, IQtComponentPainter* painter) {
            _componentPainters[componentTypeHashKey] = std::unique_ptr<IQtComponentPainter>(painter);
            _componentPaintersOrder.push_back(componentTypeHashKey);
        }

        template <typename TComponent, typename TComponentPainter, typename... TArgs>
        void RegisterComponentPainter(TArgs&&... args) {
            RegisterComponentPainter(
                ComponentTypeFromType<TComponent>(), new TComponentPainter(std::forward<TArgs>(args)...)
            );
        }

        void RegisterComponentUpdateHandler(
            ComponentTypeHashKey componentTypeHashKey, IQtComponentUpdateHandler* updateHandler
        ) {
            _componentUpdateHandlers[componentTypeHashKey] = std::unique_ptr<IQtComponentUpdateHandler>(updateHandler);
        }

        template <typename TComponent, typename TComponentUpdater, typename... TArgs>
        void RegisterComponentUpdateHandler(TArgs&&... args) {
            RegisterComponentUpdateHandler(
                ComponentTypeFromType<TComponent>(), new TComponentUpdater(std::forward<TArgs>(args)...)
            );
        }

        void Update(IEngine* engine, double deltaTime) {
            for (auto& [componentTypeKey, componentUpdateHandler] : _componentUpdateHandlers) {
                auto* updateHandler = componentUpdateHandler.get();
                entityManager()->ForEachComponent(
                    ComponentTypeFromHashKey(componentTypeKey),
                    function_pointer([this, updateHandler](
                                         Entity entity, ComponentType componentType, ComponentPointer componentPointer
                                     ) { updateHandler->Update(_engine, entity, componentPointer); })
                );
            }
        }
    };
}
