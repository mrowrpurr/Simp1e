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
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>
#include <unordered_map>
#include <unordered_set>

using namespace Simp1e;
using namespace Simp1e::ECS;

struct QtComponentUpdateHandler {
    virtual void Update(Game& game, Entity entity, ComponentPointer& component) = 0;
};

struct QtPositionComponentUpdateHandler : public QtComponentUpdateHandler {
    void Update(Game& game, Entity entity, ComponentPointer& component) override {
        _Log_("QtPositionComponentUpdateHandler::Update");
        auto* positionComponent = component_cast<PositionComponent>(component);
        if (!positionComponent) return;
        if (!positionComponent->IsDirty()) return;
        positionComponent->SetDirty(false);
        auto* graphicsItemComponent = game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
        if (!graphicsItemComponent) return;
        auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
        if (!graphicsItem) return;
        graphicsItem->setPos(ToQPointF(positionComponent->GetPosition()));
        graphicsItem->update();
    }
};

struct QtTextComponentUpdateHandler : public QtComponentUpdateHandler {
    void Update(Game& game, Entity entity, ComponentPointer& component) override {
        _Log_("QtTextComponentUpdateHandler::Update");
        auto* textComponent = component_cast<TextComponent>(component);
        if (!textComponent) return;
        auto* graphicsItemComponent = game.Entities().GetComponent<QTGraphicsItemComponent>(entity);
        if (!graphicsItemComponent) return;
        auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
        if (!graphicsItem) return;
        graphicsItem->SetText(textComponent->GetText());
        graphicsItem->update();
    }
};

struct QtComponentRenderer {
    virtual void Render(
        Game& game, Entity entity, ComponentPointer* component,
        ReadonlyEntityComponentCollection& components, QPainter* painter,
        const QStyleOptionGraphicsItem* option, QWidget* widget
    ) = 0;
};

struct QtTextComponentRenderer : public QtComponentRenderer {
    void Render(
        Game& game, Entity entity, ComponentPointer* component,
        ReadonlyEntityComponentCollection& components, QPainter* painter,
        const QStyleOptionGraphicsItem* option, QWidget* widget
    ) override {
        _Log_("QtTextComponentRenderer::Render");

        auto* rectangleComponent = components.GetComponent<RectangleComponent>();
        if (!rectangleComponent) return;

        auto* textComponent = component_cast<TextComponent>(component);
        if (!textComponent) return;

        _Log_("The text is {}", textComponent->GetText());
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
        bool somethingChanged = false;
        _Log_("QtRenderSystem::Update");
        for (auto& [componentType, componentUpdateHandler] : _componentUpdateHandlers)
            for (auto& [entityId, componentPtr] : _game.Entities().GetComponents(componentType))
                if (auto* component = static_cast<ComponentBase*>(componentPtr.get()))
                    if (component->IsDirty()) {
                        somethingChanged = true;
                        componentUpdateHandler->Update(_game, entityId, componentPtr);
                    }

        if (somethingChanged) _scene.update();
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

    QWidget window;
    window.setWindowTitle("Side Scroller");

    auto* layout = new QVBoxLayout();

    QGraphicsView  view;
    QGraphicsScene scene(0, 0, 800, 600);
    view.setScene(&scene);
    layout->addWidget(&view);

    window.show();
    ///////////////////////

    Game game;
    game.Systems().AddSystem<CommandSystem>();

    auto* qtRenderSystem = new QtRenderSystem(game, scene);
    qtRenderSystem->AddComponentUpdateHandler<PositionComponent, QtPositionComponentUpdateHandler>(
    );
    qtRenderSystem->AddComponentRenderer<TextComponent, QtTextComponentRenderer>();
    qtRenderSystem->AddComponentUpdateHandler<TextComponent, QtTextComponentUpdateHandler>();
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

    auto        labelEntityId = label.GetEntity();
    QPushButton button("Change text");
    QObject::connect(&button, &QPushButton::clicked, [&game, labelEntityId]() {
        _Log_("Changing text");
        game.Entities()
            .GetComponent<TextComponent>(labelEntityId)
            ->SetText("Hello from the entity. Updated.");
        _Log_("Updating");
        game.Update();
    });
    layout->addWidget(&button);

    window.setLayout(layout);

    // _Log_("Changing text");
    // label.GetComponent<TextComponent>()->SetText("Hello from the entity. Updated.");

    // _Log_("Updating");
    // game.Update();

    ///////////////////////

    return app.exec();
}
