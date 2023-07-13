#pragma once

#include <Simp1e/ComponentCast.h>
#include <Simp1e/IParallaxEffectComponent.h>
#include <Simp1e/ISizeComponent.h>
#include <_Log_.h>

#include "IQtComponentUpdateHandler.h"
#include "QtParallaxEffectComponent.h"

namespace Simp1e {

    class QtParallaxEffectComponentUpdateHandler : public IQtComponentUpdateHandler {
    public:
        void Update(IEngine* engine, Entity entity, void* component) override {
            auto* parallaxEffectComponent = component_cast<IParallaxEffectComponent>(component);
            if (!parallaxEffectComponent->IsDirty()) return;

            _Log_("UPDATE PARALLAX BACKGROUND");

            auto* size = engine->GetEntities()->GetComponent<ISizeComponent>(entity);
            if (!size) {
                _Log_("QtParallaxEffectComponentUpdateHandler::Update: Size component is not found!");
                return;
            }

            if (!parallaxEffectComponent) {
                _Log_("QtParallaxEffectComponentUpdateHandler::Update: Component is not a IParallaxEffectComponent!");
                return;
            }

            auto* qtParallaxEffectComponent = engine->GetEntities()->GetComponent<QtParallaxEffectComponent>(entity);
            if (!qtParallaxEffectComponent) {
                _Log_("QtParallaxEffectComponentUpdateHandler::Update: QtParallaxEffectComponent is not found!");
                return;
            }

            SynchronizeEffectDataWithUI(parallaxEffectComponent, qtParallaxEffectComponent, size->GetSize());

            parallaxEffectComponent->ClearDirty();

            // _Log_("Parallax changed!");
        }

        void SynchronizeEffectDataWithUI(
            IParallaxEffectComponent* parallaxEffectComponent, QtParallaxEffectComponent* qtParallaxEffectComponent,
            const Size& size
        ) {
            auto targetPerspectivePosition = parallaxEffectComponent->GetTargetPerspectivePosition();
            int  index                     = 0;
            parallaxEffectComponent->ForEachLayer(new_function_pointer(
                [&index, size, qtParallaxEffectComponent, targetPerspectivePosition](IParallaxEffectLayer* layer) {
                    qtParallaxEffectComponent->ConfigureLayer(index, size, targetPerspectivePosition, layer);

                    index++;
                }
            ));
        }
    };
}
