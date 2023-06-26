#include <_Log_.h>
_LogToFile_("Dynamic Library.log");
//

#include "Structures.h"

extern "C" __declspec(dllexport) void CallMe(AppStruct* app) {
    _Log_("Calling CallMe() from library");
    CoolStruct*    cool    = app->GetCoolStruct();
    AwesomeStruct* awesome = app->GetAwesomeStruct();

    _Log_("Cool number: {}", cool->CoolNumber);
    _Log_("Awesome text: {}", awesome->AwesomeText);
}
