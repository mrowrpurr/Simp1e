#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeFromType.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/Direction.h>
#include <Simp1e/ICanvasComponent.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IFillColorComponent.h>
#include <Simp1e/IImageComponent.h>
#include <Simp1e/ILabelComponent.h>
#include <Simp1e/IParallaxEffectComponent.h>
#include <Simp1e/IPositionComponent.h>
#include <Simp1e/IRectangleComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <Simp1e/IWindowComponent.h>
#include <Simp1e/IWindowMenuComponent.h>
#include <Simp1e/IWindowMenuItemComponent.h>
#include <Simp1e/QSimp1eGraphicsScene.h>
#include <Simp1e/QSimp1eGraphicsView.h>
#include <Simp1e/QWidgetComponent.h>
#include <Simp1e/QtActionComponent.h>
#include <Simp1e/QtMainWindowComponent.h>
#include <Simp1e/QtMenuComponent.h>
#include <Simp1e/QtSimp1eGraphicsItem.h>
#include <Simp1e/QtSimp1eGraphicsItemComponent.h>
#include <Simp1e/ToQRect.h>
#include <Simp1e/ToQSize.h>
#include <_Log_.h>

#include <QImageReader>
#include <QKeyEvent>
#include <QLabel>
#include <QMenuBar>
#include <QScrollBar>
#include <QStatusBar>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "IQtComponentPainter.h"
#include "IQtComponentUpdateHandler.h"
#include "QtImageComponentPainter.h"
#include "QtParallaxEffectComponent.h"
#include "QtParallaxEffectComponentUpdateHandler.h"
#include "QtPositionComponentUpdateHandler.h"
#include "QtRectangleComponentPainter.h"
#include "QtSimp1eImageComponent.h"
#include "QtSizeComponentUpdateHandler.h"

namespace Simp1e {

    class QtGuiSystem {
        IEngine* _engine;
        IFunctionPointer<void(Entity, QPainter*, const QStyleOptionGraphicsItem*, QWidget*)>*
            _entityPaintFunctionPointer;

        std::unordered_map<ComponentTypeHashKey, std::unique_ptr<IQtComponentPainter>> _componentPainters;
        std::vector<ComponentTypeHashKey>                                              _componentPaintersOrder;

        std::unordered_map<ComponentTypeHashKey, std::unique_ptr<IQtComponentUpdateHandler>> _componentUpdateHandlers;

        // TODO: these are hacks. They shouldn't be fields like this.
        QSimp1eGraphicsScene* _canvasScene;
        QSimp1eGraphicsView*  _canvasView;

        std::unordered_set<std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>> _keyPressListeners;

        IEntityManager* entityManager() const { return _engine->GetEntities(); }

        QtSimp1eGraphicsItemComponent* addGraphicsItem(Entity entity) {
            auto* component = entityManager()->GetComponent<QtSimp1eGraphicsItemComponent>(entity);
            if (component) return component;

            component = entityManager()->AddComponent<QtSimp1eGraphicsItemComponent>(
                entity, entity, _entityPaintFunctionPointer
            );

            _canvasScene->addItem(component->GetQtSimp1eGraphicsItem());

            return component;
        }

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
            auto* qtMainWindowComponent =
                entityManager()->AddComponent<QtMainWindowComponent>(entity, windowComponent->GetTitle());
            entityManager()->AddComponent<QWidgetComponent>(entity, qtMainWindowComponent->GetCentralQWidget());
        }

        void OnWindowMenuAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuAdded");
            auto* windowMenuComponent = component_cast<IWindowMenuComponent>(component);
            auto* qMainIWindowComponent =
                _engine->GetEntities()->GetComponent<QtMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qMenu = qMainIWindowComponent->GetQMainWindow()->menuBar()->addMenu(windowMenuComponent->GetText());
            entityManager()->AddComponent<QtMenuComponent>(entity, qMenu);
        }

        void OnWindowMenuItemAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> OnWindowMenuItemAdded");
            auto* windowMenuItemComponent = component_cast<IWindowMenuItemComponent>(component);
            auto* windowMenuComponent     = _engine->GetEntities()->GetComponent<IWindowMenuComponent>(
                windowMenuItemComponent->GetWindowMenuEntity()
            );
            auto* qMainIWindowComponent =
                _engine->GetEntities()->GetComponent<QtMainWindowComponent>(windowMenuComponent->GetWindowEntity());
            auto* qtMenuComponent =
                _engine->GetEntities()->GetComponent<QtMenuComponent>(windowMenuItemComponent->GetWindowMenuEntity());
            auto* qAction = qtMenuComponent->GetQMenu()->addAction(windowMenuItemComponent->GetText());
            entityManager()->AddComponent<QtActionComponent>(entity, qAction);
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
                // auto* testTempLabel = new QLabel{"CANVAS IS HERE"};
                // layout->addWidget(testTempLabel);
                auto* view  = new QSimp1eGraphicsView();
                auto* scene = new QSimp1eGraphicsScene();
                auto  size  = canvasComponent->GetSize();
                if (!size.IsNull()) scene->setSceneRect(ToQRect(size));
                view->setScene(scene);
                //
                view->move(0, 0);
                view->horizontalScrollBar()->setValue(0);
                view->verticalScrollBar()->setValue(0);
                //
                view->FitSceneToViewHeight();
                // view->FitScreenToSystemHeight();
                //
                layout->addWidget(view);

                // Save for graphical components to render on: (kinda gross, clean this up...)
                _canvasScene = scene;
                _canvasView  = view;
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
            addGraphicsItem(entity);
            auto* rectangleComponent = component_cast<IRectangleComponent>(component);
        }

        // QGraphicsItem* theShip = nullptr;  // Cause there's only 1 image right now

        void OnImageAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> ImageAdded");
            if (!_canvasScene) return;
            addGraphicsItem(entity)->GetQtSimp1eGraphicsItem();
            auto* imageComponent = component_cast<IImageComponent>(component);
            auto* qImageComponent =
                entityManager()->AddComponent<QtSimp1eImageComponent>(entity, imageComponent->GetImagePath());
        }

        void OnParallaxEffectAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> ParallaxEffectAdded");
            if (!_canvasScene) return;
            addGraphicsItem(entity);
            entityManager()->AddComponent<QtParallaxEffectComponent>(entity, _canvasScene);
        }

        void UpdateWindow(Entity entity, void* component) {
            auto windowComponent = component_cast<IWindowComponent>(component);
            if (!windowComponent->IsDirtyFlagSet(IWindowComponent::Fields::StatusBarText)) return;
            auto* qMainIWindowComponent = _engine->GetEntities()->GetComponent<QtMainWindowComponent>(entity);
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
            QImageReader::setAllocationLimit(0);

            _entityPaintFunctionPointer = new_function_pointer(this, &QtGuiSystem::OnPaintGraphicsItem);

            auto* entityEvents = entityManager()->GetEventManager();

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
            entityEvents->RegisterForComponentAdded<IParallaxEffectComponent>(
                {this, &QtGuiSystem::OnParallaxEffectAdded}
            );

            RegisterComponentPainter<IRectangleComponent, QtRectangleComponentPainter>();
            RegisterComponentPainter<IImageComponent, QtImageComponentPainter>();

            RegisterComponentUpdateHandler<IPositionComponent, QtPositionComponentUpdateHandler>();
            RegisterComponentUpdateHandler<ISizeComponent, QtSizeComponentUpdateHandler>();
            RegisterComponentUpdateHandler<IParallaxEffectComponent, QtParallaxEffectComponentUpdateHandler>();
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

        void OnKeyPress(IFunctionPointer<void(QKeyEvent*)>* functionPointer) {
            if (_canvasView) _canvasView->OnKeyPress(functionPointer);
        }

        void OnViewportEvent(IFunctionPointer<void(QEvent*)>* callback) {
            if (_canvasView) _canvasView->OnViewportEvent(callback);
        }

        void OnResize(IFunctionPointer<void()>* callback) {
            if (_canvasView) _canvasView->OnResize(callback);
        }

        void OnAccelerometerReadingChanged(IFunctionPointer<void(QAccelerometerReading*)>* callback) {
            if (_canvasView) _canvasView->OnAccelerometerReadingChanged(callback);
        }

        qreal _rotation = 0;

        void Rotate(double angle) {
            // _rotation -= angle;
            if (_canvasView) _canvasView->rotate(angle);
        }

        void CenterOn(const Position& position) {
            if (_canvasView) _canvasView->centerOn(ToQPointF(position));
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
