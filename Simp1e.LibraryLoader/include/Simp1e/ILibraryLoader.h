#pragma once

namespace Simp1e {

    struct ILibraryLoader {
        virtual void  Load(const char* libraryFilePath)                                         = 0;
        virtual void  Unload(const char* libraryFilePath)                                       = 0;
        virtual void* GetFunctionPointer(const char* libraryFilePath, const char* functionName) = 0;
    };
}
