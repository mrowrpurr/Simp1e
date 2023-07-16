#pragma once

#include <Simp1e/ComponentType.h>
#include <Simp1e/DefineSystemType.h>
#include <Simp1e/IEngine.h>

#include "IQtComponentEventHandler.h"
#include "IQtComponentPainter.h"
#include "IQtComponentUpdateHandler.h"

namespace Simp1e {

    struct UNUSED_IQtGuiSystem {
        DEFINE_SYSTEM_TYPE("QtGUI")

        virtual void Update(IEngine* engine, double deltaTime) = 0;

        virtual void RegisterComponentPainter(ComponentType componentTypey, IQtComponentPainter* painter) = 0;

        template <typename TComponent>
        void RegisterComponentPainter(IQtComponentPainter* painter) {
            RegisterComponentPainter(ComponentTypeFromType<TComponent>(), painter);
        }

        template <typename TComponent, typename... TArgs>
        void RegisterComponentPainter(TArgs&&... args) {
            RegisterComponentPainter<TComponent>(new TComponent(std::forward<TArgs>(args)...));
        }

        void RegisterComponentUpdateHandler(ComponentType componentType, IQtComponentUpdateHandler* updateHandler) = 0;

        template <typename TComponent>
        void RegisterComponentUpdateHandler(IQtComponentUpdateHandler* updateHandler) {
            RegisterComponentUpdateHandler(ComponentTypeFromType<TComponent>(), updateHandler);
        }

        template <typename TComponent, typename... TArgs>
        void RegisterComponentUpdateHandler(TArgs&&... args) {
            RegisterComponentUpdateHandler<TComponent>(new TComponent(std::forward<TArgs>(args)...));
        }
    };
}
