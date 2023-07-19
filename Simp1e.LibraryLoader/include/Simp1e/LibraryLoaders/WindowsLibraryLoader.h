#pragma once

#include <_Log_.h>

#include <filesystem>
#include <string>
#include <unordered_map>

#include "../ILibraryLoader.h"
#include "../LoadedLibraryPointer.h"

namespace Simp1e {

    constexpr auto LibraryFileExtension = ".dll";

    class WindowsLibraryLoader : public ILibraryLoader {
        std::unordered_map<std::string, LoadedLibraryPointer> _loadedLibraries;

        std::filesystem::path GetLibraryFilePath(const char* libraryBasePath) {
            std::filesystem::path libraryFilePath = libraryBasePath;
            if (std::filesystem::exists(libraryFilePath)) return libraryFilePath;
            libraryFilePath += LibraryFileExtension;
            if (std::filesystem::exists(libraryFilePath)) return libraryFilePath;
            _Log_("Library '{}' not found", libraryBasePath);
            return {};
        }

    public:
        void Load(const char* libraryBasePath) override {
            _Log_("... load ...");
            auto libraryFilePath = GetLibraryFilePath(libraryBasePath).string();
            if (libraryFilePath.empty()) return;

            if (_loadedLibraries.find(libraryFilePath) != _loadedLibraries.end()) {
                _Log_("Library '{}' already loaded", libraryFilePath);
                return;
            }
            try {
                _Log_("LoadLibraryA('{}')", libraryFilePath.c_str());
                LoadedLibraryPointer loadedLibrary = LoadLibraryA(libraryFilePath.c_str());
                if (!loadedLibrary) {
                    _Log_("Failed to load library '{}'", libraryFilePath);
                    return;
                }
                _loadedLibraries[libraryFilePath] = std::move(loadedLibrary);
                _Log_("Loaded library '{}'", libraryFilePath);
            } catch (const std::exception& e) {
                _Log_("Failed to load library '{}': {}", libraryFilePath, e.what());
            }
        }

        void Unload(const char* libraryBasePath) override {
            auto libraryFilePath = GetLibraryFilePath(libraryBasePath).string();
            if (libraryFilePath.empty()) return;

            if (_loadedLibraries.find(libraryFilePath) == _loadedLibraries.end()) {
                _Log_("Library '{}' not loaded", libraryFilePath);
                return;
            }
            try {
                if (!FreeLibrary(_loadedLibraries[libraryFilePath])) {
                    _Log_("Failed to unload library '{}'", libraryFilePath);
                    return;
                }
                _loadedLibraries.erase(libraryFilePath);
                _Log_("Unloaded library '{}'", libraryFilePath);
            } catch (const std::exception& e) {
                _Log_("Failed to unload library '{}': {}", libraryFilePath, e.what());
            }
        }

        void* GetFunctionVoidPointer(const char* libraryBasePath, const char* functionName) override {
            auto libraryFilePath = GetLibraryFilePath(libraryBasePath).string();
            if (libraryFilePath.empty()) return nullptr;

            if (_loadedLibraries.find(libraryFilePath) == _loadedLibraries.end()) {
                _Log_("Library '{}' not loaded", libraryFilePath);
                return nullptr;
            }
            try {
                _Log_("GetProcAddress({}, {})", libraryFilePath, functionName);
                FARPROC functionPointer = GetProcAddress(_loadedLibraries[libraryFilePath], functionName);
                if (!functionPointer) return nullptr;
                _Log_("Returning as void*");
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
