#include <QDebug>
#define _Log_(...) qDebug(__VA_ARGS__)
//

#include <Simp1e/ECS.h>
#include <Simp1e/ECS/CommandSystem.h>
#include <Simp1e/ECS/ComponentCast.h>
#include <Simp1e/ECS/Game.h>
#include <Simp1e/ECS/PositionComponent.h>
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

using namespace Simp1e;
using namespace Simp1e::ECS;

class QtRenderSystem {
    // TODO we need to know when entities are destroyed so we can remove the graphics item
    std::unordered_map<Entity, QTGraphicsItem*> _graphicItems;

    Game&           _game;
    QGraphicsScene& _scene;

public:
    SIMP1E_ECS_SYSTEM("QtRender")

    QtRenderSystem(Game& game, QGraphicsScene& scene) : _game(game), _scene(scene) {}

    QTGraphicsItem* FindGraphicsItem(Entity entity) {
        auto it = _graphicItems.find(entity);
        if (it != _graphicItems.end()) return it->second;
        return nullptr;
    }

    QTGraphicsItem* FindOrCreateGraphicsItem(Entity entity) {
        auto it = _graphicItems.find(entity);
        if (it != _graphicItems.end()) return it->second;
        auto* graphicsItem =
            new QTGraphicsItem([this, entity](auto* painter, auto* option, auto* widget) {
                PaintEntity(entity, painter, option, widget);
            });
        _scene.addItem(graphicsItem);
        _graphicItems[entity] = graphicsItem;
        return graphicsItem;
    }

    void UpdateText(Entity entity, TextComponent* textComponent, QTGraphicsItem* graphicsItem) {
        if (!textComponent) return;
        if (textComponent->IsDirty()) {
            textComponent->SetDirty(false);
            graphicsItem->SetText(textComponent->GetText());
        }
    }

    void UpdateRectangle(
        Entity entity, RectangleComponent* rectangleComponent, QTGraphicsItem* graphicsItem
    ) {
        if (!rectangleComponent) return;
        if (rectangleComponent->IsDirty()) {
            rectangleComponent->SetDirty(false);
            qDebug() << "UpdateRectangle";
            graphicsItem->SetSize(ToQSizeF(rectangleComponent->GetRectangle().size()));
        }
    }

    void UpdateVisible(Entity entity, VisibleComponent* visibleComponent) {
        if (!visibleComponent) return;
        if (visibleComponent->IsDirty()) {
            visibleComponent->SetDirty(false);
            auto* graphicsItem = FindOrCreateGraphicsItem(entity);
            if (visibleComponent->IsVisible()) graphicsItem->show();
            else graphicsItem->hide();

            UpdateRectangle(
                entity, _game.Entities().GetComponent<RectangleComponent>(entity), graphicsItem
            );
            UpdateText(entity, _game.Entities().GetComponent<TextComponent>(entity), graphicsItem);
        }
    }

    void PaintEntity(Entity entity, QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*) {
        qDebug() << "PaintEntity" << entity;
        auto* graphicsItem = FindGraphicsItem(entity);
        if (!graphicsItem) return;

        auto components = _game.Entities().GetComponents(entity);

        auto visible = components.GetComponent<VisibleComponent>();
        if (!visible || !visible->IsVisible()) return;

        auto rectangle = components.GetComponent<RectangleComponent>();
        if (rectangle) {
            painter->setBrush(QBrush(Qt::red));
            painter->drawRect(graphicsItem->GetBoundingRect());
        }

        auto text = components.GetComponent<TextComponent>();
        if (text) {
            painter->setPen(QPen(Qt::blue));
            painter->drawText(graphicsItem->GetBoundingRect(), ToQString(text->GetText()));
        }
    }

    void Update() {
        for (auto& [entity, component] : _game.Entities().GetComponents<VisibleComponent>())
            UpdateVisible(entity, component_cast<VisibleComponent>(component));
    }
};

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);
    app.setStyle("fusion");
    QGraphicsView  window;
    QGraphicsScene scene(0, 0, 800, 600);
    window.setScene(&scene);
    ///////////////////////

    Game game;
    game.Systems().AddSystem<CommandSystem>();
    game.Systems().AddSystem<QtRenderSystem>({game, scene});

    auto label = game.Entities().CreateEntity();
    label.AddComponent<VisibleComponent>();
    label.AddComponent<RectangleComponent>({
        {100, 100}
    });
    label.AddComponent<TextComponent>({"Hello from the entity."});
    label.AddComponent<PositionComponent>({100, 100});

    qDebug() << "Run app";

    game.Update();

    ///////////////////////
    window.setWindowTitle("Side Scroller");
    window.show();
    return app.exec();
}
