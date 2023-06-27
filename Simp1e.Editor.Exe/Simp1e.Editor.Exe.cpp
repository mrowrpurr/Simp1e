#include <_Log_.h>
_LogToFile_("Simp1e.log");
// // // // // // // // // //

#include <Simp1e\GetLibraryLoader.h>
#include <Simp1e\LibraryFileExtension.h>

#include <filesystem>
#include <memory>

int main() {
    _Log_("Simp1e startup");

    // TODO: read the paths of .dll/.so/.dylib to load from a configuration file
    auto  libraryFilePrefix    = "Simp1e";
    auto& libraryFileExtension = Simp1e::LibraryFileExtension;

    std::vector<std::filesystem::path> librariesToLoad;
    for (const auto& entry : std::filesystem::directory_iterator(".")) {
        auto& path = entry.path();
        if (path.filename().string().starts_with(libraryFilePrefix) &&
            path.filename().string().ends_with(libraryFileExtension))
            librariesToLoad.push_back(path);
    }

    if (librariesToLoad.empty()) {
        _Log_("No libraries to load, exiting");
        return 1;
    }

    auto* libraryLoader = Simp1e::GetLibraryLoader();
    if (!libraryLoader) {
        _Log_("Failed to get library loader, exiting");
        return 1;
    }

    _Log_("Loading {} libraries", librariesToLoad.size());
    for (const auto& libraryToLoad : librariesToLoad) libraryLoader->Load(libraryToLoad.string().c_str());

    _Log_("Simp1e shutdown");
    return 0;
}
