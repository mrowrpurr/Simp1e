#pragma once

#include <_Log_.h>
#include <dlfcn.h>
#include <string_format.h>

#include <filesystem>
#include <string>
#include <unordered_map>

#include "DynamicLibraryManager.h"

class DlfcnDynamicLibraryManager : public DynamicLibraryManager {
    std::unordered_map<std::string, void*> _loadedLibraries;

    std::string GetLibraryPath(const char* baseLibraryPath) { return string_format("lib{}.so", baseLibraryPath); }

public:
    void LoadLib(const char* libraryPath) override {
        _Log_("Current path: {}", std::filesystem::current_path().string());
        auto path = GetLibraryPath(libraryPath);
        if (_loadedLibraries.find(path) != _loadedLibraries.end()) return;
        if (!std::filesystem::exists(path)) throw std::runtime_error("Library does not exist");
        try {
            _Log_("Loading library: {}", path);
            auto  fullPath = std::filesystem::absolute(path);
            void* library  = dlopen(fullPath.string().c_str(), RTLD_LAZY);
            if (!library) throw std::runtime_error(string_format("Failed to load library, error: {}", dlerror()));
            _Log_("Library loaded: {}", path);
            _loadedLibraries[path] = library;
        } catch (std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void UnloadLib(const char* libraryPath) override {
        auto path = GetLibraryPath(libraryPath);
        if (_loadedLibraries.find(path) == _loadedLibraries.end()) return;
        try {
            _Log_("Unloading library: {}", path);
            dlclose(_loadedLibraries[path]);
            _Log_("Library unloaded: {}", path);
            _loadedLibraries.erase(path);
        } catch (std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void* GetFunction(const char* libraryPath, const char* functionName) override {
        auto path = GetLibraryPath(libraryPath);
        if (_loadedLibraries.find(path) == _loadedLibraries.end()) return NULL;
        try {
            _Log_("Getting function: {} from library: {}", functionName, path);
            return reinterpret_cast<void*>(dlsym(_loadedLibraries[path], functionName));
        } catch (std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }
};