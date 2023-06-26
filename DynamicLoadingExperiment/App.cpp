#include <_Log_.h>
_LogToFile_("App Library Loader.log");
//

#include <filesystem>
#include <string>

#include "DynamicLibraryManager.h"
#include "Structures.h"

#ifdef _WIN32
    #include "DynamicLibraryManager_Windows.h"
WindowsDynamicLibraryManager TheLibraryManager;
#else
    #include "DynamicLibraryManager_dlfcn.h"
DlfcnDynamicLibraryManager TheLibraryManager;
#endif

struct MyAwesomeStruct : public AwesomeStruct {
    std::string TheAwesomeText;
};

class MyAppStruct : public AppStruct {
    CoolStruct      Cool;
    MyAwesomeStruct Awesome;

public:
    MyAppStruct() {
        Cool.CoolNumber        = 42;
        Awesome.TheAwesomeText = "Hello, World!";
        Awesome.AwesomeText    = Awesome.TheAwesomeText.c_str();
    }

    CoolStruct*    GetCoolStruct() override { return &Cool; }
    AwesomeStruct* GetAwesomeStruct() override { return &Awesome; }
};

MyAppStruct App;

int main() {
    TheLibraryManager.LoadLib("ExperimentalLoadingLibrary");
    void (*CallMe)(AppStruct*) =
        reinterpret_cast<void (*)(AppStruct*)>(TheLibraryManager.GetFunction("ExperimentalLoadingLibrary", "CallMe"));
    CallMe(&App);
    return 0;
}
