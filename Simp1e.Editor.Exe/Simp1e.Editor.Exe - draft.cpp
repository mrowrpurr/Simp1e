#include <_Log_.h>
_LogToFile_("Simp1e.log");
// // // // // // // // // //

#include <Simp1e/GetLibraryLoader.h>
#include <Simp1e/LibraryFileExtension.h>
#include <Simp1e/ServiceHost.h>

#include <chrono>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <memory>

#define Log(...) _Log_("[Simp1e] " __VA_ARGS__)

std::string current_time() {
    auto              now       = std::chrono::system_clock::now();
    auto              in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

int main() {
    Log("Simp1e startup {}", current_time());

    // TODO: read the paths of .dll/.so/.dylib to load from a configuration file
    auto  libraryFilePrefix    = "Simp1e";
    auto& libraryFileExtension = Simp1e::LibraryFileExtension;
    auto  initFunctionName     = "Simp1eInit";
    auto  loadFunctionName     = "Simp1eLoad";
    auto  unloadFunctionName   = "Simp1eUnload";
    auto  startFunctionName    = "Simp1eStart";

    std::vector<std::filesystem::path> librariesToLoad;
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        auto& path = entry.path();
        if (path.filename().string().starts_with(libraryFilePrefix) &&
            path.filename().string().ends_with(libraryFileExtension))
            librariesToLoad.push_back(path);
    }

    if (librariesToLoad.empty()) {
        Log("No libraries to load, exiting");
        return 1;
    }

    auto* libraryLoader = Simp1e::GetLibraryLoader();
    if (!libraryLoader) {
        Log("Failed to get library loader, exiting");
        return 1;
    }

    Log("Loading {} libraries", librariesToLoad.size());
    for (const auto& libraryToLoad : librariesToLoad) libraryLoader->Load(libraryToLoad.string().c_str());

    Simp1e::ServiceHost serviceHost;

    Log("Initializing {} libraries", librariesToLoad.size());
    for (const auto& libraryToLoad : librariesToLoad) {
        if (auto* functionPointer =
                libraryLoader->GetFunctionPointer(libraryToLoad.string().c_str(), initFunctionName)) {
            Log("Found {} function for library '{}'", initFunctionName, libraryToLoad.string());
            if (auto* initFunctionPointer = reinterpret_cast<void (*)(Simp1e::IServiceHost*)>(functionPointer)) {
                Log("Calling {} for library '{}'", initFunctionName, libraryToLoad.string());
                initFunctionPointer(&serviceHost);
                Log("Called {} for library '{}'", initFunctionName, libraryToLoad.string());
            } else {
                Log("Failed to cast function pointer for library '{}'", libraryToLoad.string());
            }
        }
    }

    Log("Loading {} libraries", librariesToLoad.size());
    for (const auto& libraryToLoad : librariesToLoad) {
        if (auto* functionPointer =
                libraryLoader->GetFunctionPointer(libraryToLoad.string().c_str(), loadFunctionName)) {
            Log("Found {} function for library '{}'", loadFunctionName, libraryToLoad.string());
            if (auto* loadFunctionPointer = reinterpret_cast<void (*)(Simp1e::IServiceHost*)>(functionPointer)) {
                Log("Calling {} for library '{}'", loadFunctionName, libraryToLoad.string());
                loadFunctionPointer(&serviceHost);
                Log("Called {} for library '{}'", loadFunctionName, libraryToLoad.string());
            } else {
                Log("Failed to cast function pointer for library '{}'", libraryToLoad.string());
            }
        }
    }

    Log("Starting {} libraries", librariesToLoad.size());
    for (const auto& libraryToLoad : librariesToLoad) {
        if (auto* functionPointer =
                libraryLoader->GetFunctionPointer(libraryToLoad.string().c_str(), startFunctionName)) {
            Log("Found {} function for library '{}'", startFunctionName, libraryToLoad.string());
            if (auto* startFunctionPointer = reinterpret_cast<void (*)(Simp1e::IServiceHost*)>(functionPointer)) {
                Log("Calling {} for library '{}'", startFunctionName, libraryToLoad.string());
                startFunctionPointer(&serviceHost);
                Log("Called {} for library '{}'", startFunctionName, libraryToLoad.string());
            } else {
                Log("Failed to cast function pointer for library '{}'", libraryToLoad.string());
            }
        }
    }

    // TODO: main loop

    Log("Unloading {} libraries", librariesToLoad.size());
    for (const auto& libraryToLoad : librariesToLoad) {
        if (auto* functionPointer =
                libraryLoader->GetFunctionPointer(libraryToLoad.string().c_str(), unloadFunctionName)) {
            Log("Found {} function for library '{}'", unloadFunctionName, libraryToLoad.string());
            if (auto* unloadFunctionPointer = reinterpret_cast<void (*)(Simp1e::IServiceHost*)>(functionPointer)) {
                Log("Calling {} for library '{}'", unloadFunctionName, libraryToLoad.string());
                unloadFunctionPointer(&serviceHost);
                Log("Called {} for library '{}'", unloadFunctionName, libraryToLoad.string());
            } else {
                Log("Failed to cast function pointer for library '{}'", libraryToLoad.string());
            }
        }
    }

    Log("Simp1e shutdown");
    return 0;
}
