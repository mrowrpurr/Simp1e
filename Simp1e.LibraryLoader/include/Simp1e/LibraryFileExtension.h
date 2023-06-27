#pragma once

namespace Simp1e {

#ifdef _WIN32
    constexpr auto LibraryFileExtension = ".dll";
#elif __linux__
    constexpr auto LibraryFileExtension = ".so";
#elif __APPLE__
    constexpr auto LibraryFileExtension = ".dylib";
#endif
}
