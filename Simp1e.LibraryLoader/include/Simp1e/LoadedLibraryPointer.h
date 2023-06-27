#pragma once

namespace Simp1e {

#ifdef _WIN32
    #include <Windows.h>
    using LoadedLibraryPointer = HMODULE;
#else
    using LoadedLibraryPointer = void*;
#endif
}
