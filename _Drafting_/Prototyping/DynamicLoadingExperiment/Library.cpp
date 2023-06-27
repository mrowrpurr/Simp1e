#include <_Log_.h>
_LogToFile_("Dynamic Library.log");
//

#include "Structures.h"

#ifdef _WIN32
    #define EXPORT __declspec(dllexport)
#else
    #define EXPORT
#endif

extern "C" EXPORT void CallMe(AppStruct* app) {
    _Log_("Calling CallMe() from library");
    CoolStruct*    cool    = app->GetCoolStruct();
    AwesomeStruct* awesome = app->GetAwesomeStruct();

    _Log_("Cool number: {}", cool->CoolNumber);
    _Log_("Awesome text: {}", awesome->AwesomeText);
}
