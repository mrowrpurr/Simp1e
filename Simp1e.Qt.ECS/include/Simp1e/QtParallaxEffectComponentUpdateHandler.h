#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/IParallaxEffectComponent.h>
#include <_Log_.h>

#include "IQtComponentUpdateHandler.h"
#include "QtParallaxEffectComponent.h"

namespace Simp1e {

    class QtParallaxEffectComponentUpdateHandler : public IQtComponentUpdateHandler {
    public:
        void Update(IEngine* engine, Entity entity, void* component) override {
            auto* parallaxEffectComponent = component_cast<IParallaxEffectComponent>(component);
            if (!parallaxEffectComponent->IsDirty()) return;

            _Log_("Parallax changed!");

            if (!parallaxEffectComponent) {
                _Log_("QtParallaxEffectComponentUpdateHandler::Update: Component is not a IParallaxEffectComponent!");
                return;
            }

            auto* qtParallaxEffectComponent = engine->GetEntities()->GetComponent<QtParallaxEffectComponent>(entity);
            if (!qtParallaxEffectComponent) {
                _Log_("QtParallaxEffectComponentUpdateHandler::Update: QtParallaxEffectComponent is not found!");
                return;
            }

            SynchronizeEffectDataWithUI(parallaxEffectComponent, qtParallaxEffectComponent);

            parallaxEffectComponent->ClearDirty();
        }

        void SynchronizeEffectDataWithUI(
            IParallaxEffectComponent* parallaxEffectComponent, QtParallaxEffectComponent* qtParallaxEffectComponent
        ) {
            auto targetPerspectivePosition = parallaxEffectComponent->GetTargetPerspectivePosition();
            parallaxEffectComponent->ForEachLayer(
                new_function_pointer([qtParallaxEffectComponent, targetPerspectivePosition](IParallaxEffectLayer* layer
                                     ) { qtParallaxEffectComponent->ConfigureLayer(layer, targetPerspectivePosition); })
            );
        }
    };
}
