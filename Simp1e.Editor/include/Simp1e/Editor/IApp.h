#pragma once

#include <Simp1e/Data/JsonDataFile.h>
#include <Simp1e/Data/JsonDataStore.h>

#include <string>
#include <utility>
#include <vector>

namespace Simp1e::Editor {

    struct IApp {
        virtual Data::JsonDataStore& GetDataStore()      = 0;
        virtual Data::JsonDataFile&  GetActiveDataFile() = 0;

        virtual void StartUpUsingDataFiles(
            const std::vector<std::pair<std::string, bool>>& dataFilePaths
        ) = 0;
    };
}
