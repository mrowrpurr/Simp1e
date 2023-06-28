#pragma once

#include <_Log_.h>

#include <string>
#include <unordered_map>

#include "../ILibraryLoader.h"
#include "../LoadedLibraryPointer.h"

namespace Simp1e {

    class WindowsLibraryLoader : public ILibraryLoader {
        std::unordered_map<std::string, LoadedLibraryPointer> _loadedLibraries;

    public:
        void Load(const char* libraryFilePath) override {
            if (_loadedLibraries.find(libraryFilePath) != _loadedLibraries.end()) {
                _Log_("Library '{}' already loaded", libraryFilePath);
                return;
            }
            try {
                LoadedLibraryPointer loadedLibrary = LoadLibraryA(libraryFilePath);
                if (!loadedLibrary) {
                    _Log_("Failed to load library '{}'", libraryFilePath);
                    return;
                }
                _loadedLibraries[libraryFilePath] = std::move(loadedLibrary);
            } catch (const std::exception& e) {
                _Log_("Failed to load library '{}': {}", libraryFilePath, e.what());
            }
        }

        void Unload(const char* libraryFilePath) override {
            if (_loadedLibraries.find(libraryFilePath) == _loadedLibraries.end()) {
                _Log_("Library '{}' not loaded", libraryFilePath);
                return;
            }
            try {
                if (!FreeLibrary(_loadedLibraries[libraryFilePath])) {
                    _Log_("Failed to unload library '{}'", libraryFilePath);
                    return;
                }
            } catch (const std::exception& e) {
                _Log_("Failed to unload library '{}': {}", libraryFilePath, e.what());
            }
        }

        void* GetFunctionPointer(const char* libraryFilePath, const char* functionName) override {
            if (_loadedLibraries.find(libraryFilePath) == _loadedLibraries.end()) {
                _Log_("Library '{}' not loaded", libraryFilePath);
                return nullptr;
            }
            try {
                FARPROC functionPointer = GetProcAddress(_loadedLibraries[libraryFilePath], functionName);
                if (!functionPointer) return nullptr;
                return reinterpret_cast<void*>(functionPointer);
            } catch (const std::exception& e) {
                _Log_(
                    "Failed to get function pointer '{}' from library '{}': {}", functionName, libraryFilePath, e.what()
                );
                return nullptr;
            }
        }
    };
}
