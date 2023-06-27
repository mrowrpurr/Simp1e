#pragma once

#include "ILibraryLoader.h"

#ifdef _WIN32
    #include "LibraryLoaders/WindowsLibraryLoader.h"
namespace Simp1e {
    ILibraryLoader* GetLibraryLoader() { return new WindowsLibraryLoader(); }
}
#elif __linux__
constexpr auto LibraryFileExtension = ".so";
#elif __APPLE__
constexpr auto LibraryFileExtension = ".dylib";
#endif
