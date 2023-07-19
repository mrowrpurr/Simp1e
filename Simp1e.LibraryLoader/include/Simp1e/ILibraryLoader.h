#pragma once

namespace Simp1e {

    namespace ILibraryLoaderHelper {
        template <typename T>
        struct function_traits;

        template <typename Ret, typename... Args>
        struct function_traits<Ret(Args...)> {
            using pointer = Ret (*)(Args...);
        };
    }

    struct ILibraryLoader {
        virtual void  Load(const char* libraryFilePath)                                             = 0;
        virtual void  Unload(const char* libraryFilePath)                                           = 0;
        virtual void* GetFunctionVoidPointer(const char* libraryFilePath, const char* functionName) = 0;

        template <typename Func>
        typename ILibraryLoaderHelper::function_traits<Func>::pointer GetFunctionPointer(
            const char* libraryFilePath, const char* functionName
        ) {
            auto* fn = GetFunctionVoidPointer(libraryFilePath, functionName);
            if (!fn) return nullptr;
            return reinterpret_cast<typename ILibraryLoaderHelper::function_traits<Func>::pointer>(fn);
        }
    };

}
