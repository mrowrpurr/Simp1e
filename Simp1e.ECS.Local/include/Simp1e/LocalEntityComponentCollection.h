#pragma once

#include <Simp1e/ComponentType.h>
#include <Simp1e/ComponentTypeHashKey.h>
#include <Simp1e/Entity.h>
#include <Simp1e/IEntityComponentCollection.h>
#include <void_pointer.h>

#include <unordered_map>

namespace Simp1e {

    class LocalEntityComponentCollection : public IEntityComponentCollection {
        std::unordered_map<ComponentTypeHashKey, VoidPointer*> _entityComponents;

        LocalEntityComponentCollection(const LocalEntityComponentCollection&)            = delete;
        LocalEntityComponentCollection& operator=(const LocalEntityComponentCollection&) = delete;

    public:
        LocalEntityComponentCollection() = default;

        void AddComponentPointer(ComponentType componentType, VoidPointer* componentPointer) {
            _entityComponents[componentType] = componentPointer;
        }

        bool HasComponent(ComponentType componentType) const {
            return _entityComponents.find(componentType) != _entityComponents.end();
        }

        bool RemoveComponent(ComponentType componentType) {
            auto found = _entityComponents.find(componentType);
            if (found == _entityComponents.end()) return false;
            _entityComponents.erase(found);
            return true;
        }

        void* GetComponentPointer(ComponentType componentType) const {
            auto found = _entityComponents.find(componentType);
            if (found == _entityComponents.end()) return nullptr;
            return found->second->get()->void_ptr();
        }
    };
}
