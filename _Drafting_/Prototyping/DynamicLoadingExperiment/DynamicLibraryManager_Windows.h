#pragma once

#include <_Log_.h>
#include <windows.h>

#include <filesystem>
#include <string>
#include <unordered_map>

#include "DynamicLibraryManager.h"

class WindowsDynamicLibraryManager : public DynamicLibraryManager {
    std::unordered_map<std::string, HMODULE> _loadedLibraries;

public:
    void LoadLib(const char* libraryPath) override {
        std::string path(libraryPath);
        path.append(".dll");
        if (_loadedLibraries.find(path) != _loadedLibraries.end()) return;

        if (!std::filesystem::exists(path)) throw std::runtime_error("Library does not exist");

        try {
            _Log_("Loading library: {}", path);
            HMODULE library = LoadLibraryA(path.c_str());
            if (library == NULL) throw std::runtime_error("Failed to load library");
            _Log_("Library loaded: {}", path);
            _loadedLibraries[path] = library;
        } catch (std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void UnloadLib(const char* libraryPath) override {
        std::string path(libraryPath);
        path.append(".dll");

        if (_loadedLibraries.find(path) == _loadedLibraries.end()) return;

        try {
            _Log_("Unloading library: {}", path);
            FreeLibrary(_loadedLibraries[path]);
            _Log_("Library unloaded: {}", path);
            _loadedLibraries.erase(path);
        } catch (std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }

    void* GetFunction(const char* libraryPath, const char* functionName) override {
        std::string path(libraryPath);
        path.append(".dll");

        if (_loadedLibraries.find(path) == _loadedLibraries.end()) return NULL;

        try {
            _Log_("Getting function: {} from library: {}", functionName, path);
            return reinterpret_cast<void*>(GetProcAddress(_loadedLibraries[path], functionName));
        } catch (std::exception& e) {
            throw std::runtime_error(e.what());
        }
    }
};