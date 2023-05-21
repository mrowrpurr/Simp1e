#pragma once

#include <string>
#include <utility>
#include <vector>

namespace Simp1e::Editor {

    struct IApp {
        // Update this cause we need to know which ones are Active
        virtual void StartUpUsingDataFiles(
            const std::vector<std::pair<std::string, bool>>& dataFilePaths
        ) = 0;
    };
}
