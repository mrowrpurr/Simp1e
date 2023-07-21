#pragma once

namespace Simp1e {

    struct INativePlugin {
        virtual ~INativePlugin() = default;

        virtual const char* GetIdentifier() const = 0;
    };
}
