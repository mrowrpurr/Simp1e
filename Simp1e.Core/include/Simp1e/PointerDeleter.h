#pragma once

namespace Simp1e {
    // TODO - make the struct, itself, templated. Not the () operator.
    struct PointerDeleter {
        template <typename T>
        void operator()(T* ptr) const {
            delete ptr;
        }
    };
}

// TODO use this instead:
// template <typename T>
// struct PointerDeleter {
//     void operator()(T* ptr) const {
//         delete ptr;
//     }
// };
//
// And use it like this:
// std::unique_ptr<MyClass, PointerDeleter<MyClass>> ptr(new MyClass);
