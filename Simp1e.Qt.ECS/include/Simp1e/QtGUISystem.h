#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/ComponentTypeFromHashKey.h>
#include <Simp1e/ComponentTypeFromType.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/Direction.h>
#include <Simp1e/ICameraComponent.h>
#include <Simp1e/ICanvasComponent.h>
#include <Simp1e/IEngine.h>
#include <Simp1e/IFillColorComponent.h>
#include <Simp1e/IImageComponent.h>
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
#include <QMenuBar>
#include <QScrollBar>
#include <QStatusBar>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "IQtComponentPainter.h"
#include "IQtComponentUpdateHandler.h"
#include "QtCameraComponentUpdateHandler.h"
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

        // TODO: global window / scene / view
        QSimp1eGraphicsScene* _guiGlobalScene;
        QSimp1eGraphicsView*  _guiGlobalView;
        QMainWindow*          _guiGlobalWindow;

        std::unordered_set<std::unique_ptr<IFunctionPointer<void(QKeyEvent*)>>> _keyPressListeners;

        IEntityManager* entityManager() const { return _engine->GetEntities(); }

        QtSimp1eGraphicsItemComponent* addGraphicsItem(Entity entity) {
            auto* component = entityManager()->GetComponent<QtSimp1eGraphicsItemComponent>(entity);
            if (component) return component;

            component = entityManager()->AddComponent<QtSimp1eGraphicsItemComponent>(
                entity, entity, _entityPaintFunctionPointer
            );

            _guiGlobalScene->addItem(component->GetQtSimp1eGraphicsItem());

            return component;
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
            _guiGlobalWindow = qtMainWindowComponent->GetQMainWindow();
            //
            qtMainWindowComponent->GetQMainWindow()->resize(1200, 900);
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

        void OnCanvasAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> CanvasAdded");
            auto* canvasComponent = component_cast<ICanvasComponent>(component);
            auto* view            = new QSimp1eGraphicsView();
            auto* scene           = new QSimp1eGraphicsScene();
            auto* size            = entityManager()->GetComponent<ISizeComponent>(entity);
            if (size) scene->setSceneRect(ToQRectF(Rectangle(Point{0, 0}, size->GetSize())));
            view->setScene(scene);
            _guiGlobalWindow->centralWidget()->layout()->addWidget(view);
            _guiGlobalScene = scene;
            _guiGlobalView  = view;
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

        void OnCameraAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> CameraAdded");
            auto* cameraComponent = component_cast<ICameraComponent>(component);
            cameraComponent->SetDirty(true);  // Start off in need of an update.
            // Need to wait until we have the view before registering the update handler.
            RegisterComponentUpdateHandler<ICameraComponent, QtCameraComponentUpdateHandler>(_guiGlobalView);
        }

        void OnRectangleAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> RectangleAdded");
            if (!_guiGlobalScene) return;
            addGraphicsItem(entity);
            auto* rectangleComponent = component_cast<IRectangleComponent>(component);
        }

        void OnImageAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> ImageAdded");
            if (!_guiGlobalScene) return;
            addGraphicsItem(entity);
            auto* imageComponent = component_cast<IImageComponent>(component);
            auto* qImageComponent =
                entityManager()->AddComponent<QtSimp1eImageComponent>(entity, imageComponent->GetImagePath());
            auto* sizeComponent = entityManager()->GetComponent<ISizeComponent>(entity);
            if (sizeComponent) qImageComponent->GetQSimp1eImage()->SetSize(sizeComponent->GetSize());
        }

        void OnParallaxEffectAdded(Entity entity, ComponentType componentType, void* component) {
            _Log_("-> ParallaxEffectAdded");
            if (!_guiGlobalScene) return;
            addGraphicsItem(entity);
            entityManager()->AddComponent<QtParallaxEffectComponent>(entity, _guiGlobalScene);
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

        // TODO: move all of these default handlers/painters/etc into includes which, when included, do a registration
        // TODO: and/or a subclass of QtGuiSystem with various defaults.
        //
        // And a IQtGuiSystem maybe. I just want all the Register*() functions someplace by themselves.
        QtGuiSystem(IEngine* engine) : _engine(engine) {
            QImageReader::setAllocationLimit(0);  // TODO: move this to the QSimp1eApp

            _entityPaintFunctionPointer = new_function_pointer(this, &QtGuiSystem::OnPaintGraphicsItem);

            auto* entityEvents = entityManager()->GetEventManager();

            entityEvents->RegisterForComponentAdded<IWindowComponent>({this, &QtGuiSystem::OnWindowAdded});
            entityEvents->RegisterForComponentAdded<IWindowMenuComponent>({this, &QtGuiSystem::OnWindowMenuAdded});
            entityEvents->RegisterForComponentAdded<IWindowMenuItemComponent>(
                {this, &QtGuiSystem::OnWindowMenuItemAdded}
            );
            entityEvents->RegisterForComponentAdded<ICanvasComponent>({this, &QtGuiSystem::OnCanvasAdded});
            entityEvents->RegisterForComponentAdded<IRectangleComponent>({this, &QtGuiSystem::OnRectangleAdded});
            entityEvents->RegisterForComponentAdded<IPositionComponent>({this, &QtGuiSystem::OnPositionAdded});
            entityEvents->RegisterForComponentAdded<ISizeComponent>({this, &QtGuiSystem::OnSizeAdded});
            entityEvents->RegisterForComponentAdded<IImageComponent>({this, &QtGuiSystem::OnImageAdded});
            entityEvents->RegisterForComponentAdded<IParallaxEffectComponent>(
                {this, &QtGuiSystem::OnParallaxEffectAdded}
            );
            entityEvents->RegisterForComponentAdded<ICameraComponent>({this, &QtGuiSystem::OnCameraAdded});

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

        int iteration = 0;

        void Update(IEngine* engine, double deltaTime) {
            iteration++;
            _engine->GetEntities()->ForEach<ICameraComponent>(
                new_function_pointer([this](Entity, ComponentType, ComponentPointer ptr) {
                    if (iteration % 100 != 0) return;
                    _Log_("Dirty Camera");
                    auto* camera = component_cast<ICameraComponent>(ptr);
                    if (camera) camera->SetDirty(true);
                })
            );

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
