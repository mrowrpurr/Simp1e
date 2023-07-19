// Simplest possible System and Component.
//
// And we won't even share them in a header!
//
// And yet we'll make it work somehow ;)

#include <_Log_.h>

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

// Normally we'd get this from a dependency
// but we'll just hard-code it here as an example that we can.
//
// Note of course that the layout must match exactly with the real struct.
struct EcsLoaderAPI {
    void (*RegisterSystemFactoryFunction)(const char* name, void* (*factoryFunction)(void*));
};

struct FooService {
    int value = 123;
};

void* GenerateFooService(void*) { return new FooService{}; }

extern "C" {
    bool EXPORT Simp1e_ECS_Load(EcsLoaderAPI* api) {
        _Log_("Called Simp1e_ECS_Load");
        if (!api) {
            _Log_("api is null!");
            return false;
        }
        // api->RegisterSystemFactoryFunction("FooService", GenerateFooService);
        // _Log_("Registered FooService");
        return true;
    }

    bool EXPORT Simp1e_ECS_Unload() { return true; }
}
