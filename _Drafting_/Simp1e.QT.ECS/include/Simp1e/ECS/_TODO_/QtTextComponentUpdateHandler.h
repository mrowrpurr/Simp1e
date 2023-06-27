#pragma once

#include <Simp1e/ECS/Simp1eQGraphicsItemComponent.h>
#include <Simp1e/ECS/TextComponent.h>

#include "QtComponentUpdateHandler.h"

namespace Simp1e::ECS {

    struct QtTextComponentUpdateHandler : public QtComponentUpdateHandler {
        void Update(Game& game, Entity entity, ComponentPointer& component) override {
            auto* textComponent = component_cast<TextComponent>(component);
            if (!textComponent) return;
            auto* graphicsItemComponent = game.Entities().GetComponent<Simp1eQGraphicsItemComponent>(entity);
            if (!graphicsItemComponent) return;
            auto* graphicsItem = graphicsItemComponent->GetGraphicsItem();
            if (!graphicsItem) return;
            graphicsItem->SetText(textComponent->GetText());
            graphicsItem->update();
        }
    };
}
