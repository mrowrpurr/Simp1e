#include <string_format.h>

#include <QDebug>

#define _Log_(...) qDebug(string_format(__VA_ARGS__).c_str())
//

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/PositionComponent.h>
#include <Simp1e/ECS/QTGraphicsItemComponent.h>
#include <Simp1e/ECS/RectangleComponent.h>
#include <Simp1e/ECS/TextComponent.h>
#include <Simp1e/ECS/VisibleComponent.h>
#include <Simp1e/QT/Conversions.h>
#include <Simp1e/QT/QTGraphicsItem.h>

#include <QApplication>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <unordered_map>
#include <unordered_set>

using namespace Simp1e;
using namespace Simp1e::ECS;

struct QtComponentUpdateHandler {
    virtual void Update(Game& game, Entity entity, ComponentPointer& component) = 0;
};

struct PositionComponentUpdateHandler : public QtComponentUpdateHandler {
    void Update(Game& game, Entity entity, ComponentPointer& component) override {
        _Log_("PositionComponentUpdateHandler::Update");
        auto* positionComponent = component_cast<PositionComponent>(component);
        _Log_("A");
        if (!positionComponent) return;
        _Log_("B");
        if (!positionComponent->IsDirty()) return;
        _Log_("C");
        positionComponent->SetDirty(false);
        auto* graphicsItemComponent = game.Entities().GetComponent<QTGraphicsItemComponent>(entity);

        if (!graphicsItemComponent) return;
        _Log_("D");
        auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
        if (!graphicsItem) return;
        _Log_("E");
        graphicsItem->setPos(ToQPointF(positionComponent->GetPosition()));
        graphicsItem->update();
        _Log_("PositionComponentUpdateHandler::Update - done");
    }
};

struct QtComponentRenderer {
    virtual void Render(
        Game& game, Entity entity, ComponentPointer* component,
        ReadonlyEntityComponentCollection& components, QPainter* painter,
        const QStyleOptionGraphicsItem* option, QWidget* widget
    ) = 0;
};

struct TextComponentRenderer : public QtComponentRenderer {
    void Render(
        Game& game, Entity entity, ComponentPointer* component,
        ReadonlyEntityComponentCollection& components, QPainter* painter,
        const QStyleOptionGraphicsItem* option, QWidget* widget
    ) override {
        _Log_("TextComponentRenderer::Render");

        auto* rectangleComponent = components.GetComponent<RectangleComponent>();
        if (!rectangleComponent) return;

        auto* textComponent = component_cast<TextComponent>(component);
        if (!textComponent) return;

        painter->drawText(
            ToQRectF(rectangleComponent->GetRectangle()), ToQString(textComponent->GetText())
        );
    }
};

// TODO - make this a Manager, not a System.
class QtRenderSystem {
    Game&                                                                   _game;
    QGraphicsScene&                                                         _scene;
    std::unordered_set<ComponentType>                                       _visualComponentTypes;
    std::unordered_map<ComponentType, std::unique_ptr<QtComponentRenderer>> _componentRenderers;
    std::unordered_map<ComponentType, std::unique_ptr<QtComponentUpdateHandler>>
        _componentUpdateHandlers;

    // Delete copy constructor
    QtRenderSystem(const QtRenderSystem&) = delete;

public:
    SIMP1E_ECS_SYSTEM("QtRender")

    QtRenderSystem(Game& game, QGraphicsScene& scene) : _game(game), _scene(scene) {
        _Log_("QtRenderSystem::QtRenderSystem");
        _game.Entities().Events().OnComponentAdded<VisibleComponent>([this, &scene](Entity entity) {
            auto* graphicsItemComponent = new QTGraphicsItemComponent(
                scene,
                [this, entity](
                    QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
                ) { PaintEntity(entity, painter, option, widget); }
            );
            _game.Entities().AddComponent<QTGraphicsItemComponent>(entity, graphicsItemComponent);
        });
        _game.Entities().Events().OnComponentAdded([this](auto entity, auto& componentType) {
            _Log_("QtRenderSystem::OnComponentAdded");
            if (!this->_visualComponentTypes.count(componentType)) return;
            auto* graphicsItemComponent =
                _game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
            if (graphicsItemComponent) graphicsItemComponent->update();
        });
        _game.Entities().Events().OnComponentRemoved([this](auto entity, auto& componentType) {
            if (!this->_visualComponentTypes.count(componentType)) return;
            auto* graphicsItemComponent =
                _game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
            if (graphicsItemComponent) graphicsItemComponent->update();
        });
        _game.Entities().Events().OnComponentRemoved<VisibleComponent>([this](Entity entity) {
            _game.Entities().RemoveComponent<QTGraphicsItemComponent>(entity);
        });
    }

    void AddVisualComponentType(ComponentType componentType) {
        _visualComponentTypes.insert(componentType);
    }

    template <typename T>
    void AddVisualComponentType() {
        AddVisualComponentType(T::GetComponentType());
    }

    void AddComponentRenderer(ComponentType componentType, QtComponentRenderer* renderer) {
        _componentRenderers[componentType] = std::unique_ptr<QtComponentRenderer>(renderer);
        AddVisualComponentType(componentType);
    }

    template <typename T>
    void AddComponentRenderer(QtComponentRenderer* renderer) {
        AddComponentRenderer(T::GetComponentType(), renderer);
    }

    template <typename T, typename R>
    void AddComponentRenderer() {
        AddComponentRenderer(T::GetComponentType(), new R());
    }

    void AddComponentUpdateHandler(
        ComponentType componentType, QtComponentUpdateHandler* updateHandler
    ) {
        _componentUpdateHandlers[componentType] =
            std::unique_ptr<QtComponentUpdateHandler>(updateHandler);
        AddVisualComponentType(componentType);
    }

    template <typename T>
    void AddComponentUpdateHandler(QtComponentUpdateHandler* updateHandler) {
        AddComponentUpdateHandler(T::GetComponentType(), updateHandler);
    }

    template <typename T, typename U>
    void AddComponentUpdateHandler() {
        AddComponentUpdateHandler(T::GetComponentType(), new U());
    }

    void Update() {
        _Log_("QtRenderSystem::Update");
        for (auto& [componentType, componentUpdateHandler] : _componentUpdateHandlers)
            for (auto& [entityId, componentPtr] : _game.Entities().GetComponents(componentType))
                if (auto* component = static_cast<ComponentBase*>(componentPtr.get()))
                    if (component->IsDirty())
                        componentUpdateHandler->Update(_game, entityId, componentPtr);
    }

    void PaintEntity(
        Entity entityId, QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget
    ) {
        _Log_("QtRenderSystem::PaintEntity {}", entityId);
        auto entity = _game.Entities().Get(entityId);

        auto components = entity.GetComponents();
        _Log_("Entity {} has {} components", entityId, components.size());

        // TODO have some kind of way to check the VisibleComponent and not do this if it's not
        // visible

        for (auto& [componentType, component] : components) {
            _Log_("QtRenderSystem::PaintEntity::Component {}", componentType.c_str());
            if (!_visualComponentTypes.count(componentType)) continue;
            auto foundRenderer = _componentRenderers.find(componentType);
            if (foundRenderer == _componentRenderers.end()) continue;
            foundRenderer->second->Render(
                _game, entityId, component, components, painter, option, widget
            );
        }
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("fusion");
    QGraphicsView  window;
    QGraphicsScene scene(0, 0, 800, 600);
    window.setScene(&scene);
    window.setWindowTitle("Side Scroller");
    window.show();
    ///////////////////////

    Game game;
    game.Systems().AddSystem<CommandSystem>();

    auto* qtRenderSystem = new QtRenderSystem(game, scene);
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, PositionComponentUpdateHandler>();
    qtRenderSystem->AddVisualComponentType<RectangleComponent>();
    qtRenderSystem->AddComponentRenderer<TextComponent, TextComponentRenderer>();
    game.Systems().AddSystem(qtRenderSystem);

    auto label = game.Entities().CreateEntity();
    label.AddComponent<VisibleComponent>();
    label.AddComponent<RectangleComponent>({
        {100, 100}
    });
    label.AddComponent<TextComponent>({"Hello from the entity."});
    label.AddComponent<PositionComponent>({100, 100});

    qDebug() << "Run app";

    game.Update();

    _Log_("Changing position");
    // label.GetComponent<PositionComponent>()->SetPosition({200, 200});

    _Log_("Updating");
    game.Update();

    _Log_("Changing text");
    label.GetComponent<TextComponent>()->SetText("Hello from the entity. Updated.");

    // _Log_("Updating");
    // game.Update();

    ///////////////////////

    return app.exec();
}

// class QtRenderSystem {
//     // TODO we need to know when entities are destroyed so we can remove the graphics item
//     std::unordered_map<Entity, QTGraphicsItem*> _graphicItems;

//     Game&           _game;
//     QGraphicsScene& _scene;

// public:
//     SIMP1E_ECS_SYSTEM("QtRender")

//     QtRenderSystem(Game& game, QGraphicsScene& scene) : _game(game), _scene(scene) {}

//     QTGraphicsItem* FindGraphicsItem(Entity entity) {
//         auto it = _graphicItems.find(entity);
//         if (it != _graphicItems.end()) return it->second;
//         return nullptr;
//     }

//     QTGraphicsItem* FindOrCreateGraphicsItem(Entity entity) {
//         auto it = _graphicItems.find(entity);
//         if (it != _graphicItems.end()) return it->second;
//         auto* graphicsItem =
//             new QTGraphicsItem([this, entity](auto* painter, auto* option, auto* widget) {
//                 PaintEntity(entity, painter, option, widget);
//             });
//         _scene.addItem(graphicsItem);
//         _graphicItems[entity] = graphicsItem;
//         return graphicsItem;
//     }

//     void UpdateText(Entity entity, TextComponent* textComponent, QTGraphicsItem* graphicsItem) {
//         if (!textComponent) return;
//         if (textComponent->IsDirty()) {
//             textComponent->SetDirty(false);
//             graphicsItem->SetText(textComponent->GetText());
//         }
//     }

//     void UpdateRectangle(
//         Entity entity, RectangleComponent* rectangleComponent, QTGraphicsItem* graphicsItem
//     ) {
//         if (!rectangleComponent) return;
//         if (rectangleComponent->IsDirty()) {
//             rectangleComponent->SetDirty(false);
//             qDebug() << "UpdateRectangle";
//             graphicsItem->SetSize(ToQSizeF(rectangleComponent->GetRectangle().size()));
//         }
//     }

//     void UpdateVisible(Entity entity, VisibleComponent* visibleComponent) {
//         if (!visibleComponent) return;
//         if (visibleComponent->IsDirty()) {
//             visibleComponent->SetDirty(false);
//             auto* graphicsItem = FindOrCreateGraphicsItem(entity);
//             if (visibleComponent->IsVisible()) graphicsItem->show();
//             else graphicsItem->hide();

//             UpdateRectangle(
//                 entity, _game.Entities().GetComponent<RectangleComponent>(entity), graphicsItem
//             );
//             UpdateText(entity, _game.Entities().GetComponent<TextComponent>(entity),
//             graphicsItem);
//         }
//     }

//     void PaintEntity(Entity entity, QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
//     {
//         qDebug() << "PaintEntity" << entity;
//         auto* graphicsItem = FindGraphicsItem(entity);
//         if (!graphicsItem) return;

//         auto components = _game.Entities().GetComponents(entity);

//         auto visible = components.GetComponent<VisibleComponent>();
//         if (!visible || !visible->IsVisible()) return;

//         auto rectangle = components.GetComponent<RectangleComponent>();
//         if (rectangle) {
//             painter->setBrush(QBrush(Qt::red));
//             painter->drawRect(graphicsItem->GetBoundingRect());
//         }

//         auto text = components.GetComponent<TextComponent>();
//         if (text) {
//             painter->setPen(QPen(Qt::blue));
//             painter->drawText(graphicsItem->GetBoundingRect(), ToQString(text->GetText()));
//         }
//     }

//     void Update() {
//         for (auto& [entity, component] : _game.Entities().GetComponents<VisibleComponent>())
//             UpdateVisible(entity, component_cast<VisibleComponent>(component));
//     }
// };