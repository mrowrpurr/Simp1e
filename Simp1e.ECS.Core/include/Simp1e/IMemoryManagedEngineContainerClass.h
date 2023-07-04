#pragma once

namespace Simp1e {
    struct IMemoryManagedEngineContainerClass {
        virtual ~IMemoryManagedEngineContainerClass() = default;
        virtual bool ManagesEngineItemMemory() const  = 0;
    };
}
