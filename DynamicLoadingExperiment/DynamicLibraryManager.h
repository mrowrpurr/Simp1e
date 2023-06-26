#pragma once

struct DynamicLibraryManager {
    virtual ~DynamicLibraryManager()                                             = default;
    virtual void  LoadLib(const char* libraryPath)                               = 0;
    virtual void  UnloadLib(const char* libraryPath)                             = 0;
    virtual void* GetFunction(const char* libraryPath, const char* functionName) = 0;
};
