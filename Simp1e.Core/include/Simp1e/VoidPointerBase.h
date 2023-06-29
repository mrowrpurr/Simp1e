#pragma once

namespace Simp1e {

    struct VoidPointerBase {
        virtual void* void_pointer() const                            = 0;
        virtual void  set_destruct_on_delete(bool destruct_on_delete) = 0;
        virtual bool  does_destruct_on_delete() const                 = 0;
        virtual ~VoidPointerBase()                                    = default;

        template <typename T>
        T* as() const {
            return static_cast<T*>(void_pointer());
        }
    };
}
