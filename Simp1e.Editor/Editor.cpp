#include <_Log_.h>
_LogToFile_("Editor.log");
//

#include <string_format.h>

#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "Simp1e/Editor/Main.h"

int main(int argc, char* argv[]) {
    // Print out the current date and time
    auto              now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::stringstream nowStringStream;
    nowStringStream << std::put_time(std::localtime(&now), "%m-%d-%y %H:%M:%S");
    _Log_("Starting Simp1e.Editor at {}", nowStringStream.str());

    return Simp1e::Editor::Main(argc, argv);
}

// Definitions

namespace Simp1e::Editor::Paths::Development {
    std::filesystem::path ProjectFolder{"../../../../"};
    std::filesystem::path ResourcesFolder{ProjectFolder / "Resources"};
    std::filesystem::path DataFolder{ResourcesFolder / "Data"};
    std::filesystem::path QssFile{ResourcesFolder / "Editor" / "Editor.qss"};
}
