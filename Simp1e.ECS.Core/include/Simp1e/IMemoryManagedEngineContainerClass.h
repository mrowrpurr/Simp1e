#pragma once

namespace Simp1e {

    // TODO - get rid of this, overly complicated and fancy
    struct IMemoryManagedEngineContainerClass {
        virtual ~IMemoryManagedEngineContainerClass() = default;
        virtual bool ManagesEngineItemMemory() const  = 0;
    };
}
