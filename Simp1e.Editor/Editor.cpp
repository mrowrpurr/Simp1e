#include <_Log_.h>
_LogToFile_("Editor.log");
//

#include "Simp1e/Editor/Main.h"

int main(int argc, char* argv[]) { return Simp1e::Editor::Main(argc, argv); }

// Definitions

namespace Simp1e::Editor::Paths::Development {
    std::filesystem::path ProjectFolder{"../../../../"};
    std::filesystem::path ResourcesFolder{ProjectFolder / "Resources"};
    std::filesystem::path DataFolder{ResourcesFolder / "Data"};
    std::filesystem::path QssFile{ResourcesFolder / "Editor" / "Editor.qss"};
}
