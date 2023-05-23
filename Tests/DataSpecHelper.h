#pragma once

#include "SpecHelper.h"
//

#include <Simp1e/Data/JsonDataFile.h>
#include <Simp1e/Data/JsonDataStore.h>

using namespace Simp1e::Data;

inline std::unique_ptr<Simp1e::Data::JsonDataFile> GetJsonDataFile(std::string filename) {
    return std::make_unique<Simp1e::Data::JsonDataFile>(
        std::filesystem::path("Tests/Resources/DataFiles") / filename
    );
}

inline std::unique_ptr<Simp1e::Data::JsonDataStore> GetJsonDataStore(std::string filename) {
    auto dataFile  = GetJsonDataFile(filename);
    auto dataStore = new JsonDataStore();
    dataStore->InsertDataFile(*dataFile);
    return std::unique_ptr<JsonDataStore>(dataStore);
}