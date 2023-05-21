#pragma once

#include <vector>

#include "Record.h"

namespace Simp1e::Data {

    struct DataStore {
        virtual ~DataStore() = default;

        virtual bool HasObject(const char* fullIdentifier) = 0;

        virtual Record* GetRecord(const char* fullIdentifier)                            = 0;
        virtual Record* GetRecord(const char* ownerName, const char* relativeIdentifier) = 0;

        virtual bool DeleteRecord(const char* fullIdentifier) = 0;

        virtual std::vector<Record*> GetAllRecords()                             = 0;
        virtual std::vector<Record*> GetAllRecordsOfOwner(const char* ownerName) = 0;
        virtual std::vector<Record*> GetAllRecordsOfType(const char* typeName)   = 0;
    };
}
