#pragma once

#include <Specs/DSL/BDD.h>
#include <snowhouse/snowhouse.h>

#include <filesystem>
#include <functional>
#include <memory>

using namespace snowhouse;

#define Assert "Do not use Assert::That"

namespace HacksPendingSpecsCppLibraryCompletion {
    extern std::vector<std::function<void()>> SpecDefinitionBodies;

    struct CollectSpecDefinitionBody {
        CollectSpecDefinitionBody(std::function<void()> body);
    };
}

#define __SpecsHack_Macro_Concat_(x, y) x##y
#define __SpecsHack_Macro_Concat(x, y) __SpecsHack_Macro_Concat_(x, y)
#define __SpecsHack_Macro_Unique(count) \
    __SpecsHack_Macro_Concat(__SpecsHack_UniqueCppFileValue2, count)

#define __SpecsHack_Macro_LambdaCollector(count)                                               \
    HacksPendingSpecsCppLibraryCompletion::CollectSpecDefinitionBody __SpecsHack_Macro_Concat( \
        __SpecsHack_UniqueCppFileValue, count                                                  \
    )

#define __SpecsHack_Macro_CollectSpecs(count)                                              \
    inline void __SpecsHack_Macro_Unique(count)();                                         \
    __SpecsHack_Macro_LambdaCollector(count){[]() { __SpecsHack_Macro_Unique(count)(); }}; \
    inline void __SpecsHack_Macro_Unique(count)()

#define Tests __SpecsHack_Macro_CollectSpecs(__COUNTER__)

#define __SpecsHack_Macro_DefineDescribe(description, count)                         \
    inline void __SpecsHack_Macro_Unique(count)();                                   \
    __SpecsHack_Macro_LambdaCollector(count                                          \
    ){[]() { describe(description, []() { __SpecsHack_Macro_Unique(count)(); }); }}; \
    inline void __SpecsHack_Macro_Unique(count)()

#define Describe(description) __SpecsHack_Macro_DefineDescribe(description, __COUNTER__)
