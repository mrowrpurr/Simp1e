#pragma once

#include <Simp1e/Data/Record.h>

namespace Simp1e::Data {

    class MapRecord {
        Data::Record* _record;

    public:
        MapRecord(Data::Record* record) : _record(record) {}

        Data::Record*     GetRecord() { return _record; }
        Data::RecordData* GetData() { return _record->GetData(); }

        const char* GetID() { return _record->GetFullIdentifier(); }
        int         GetRows() { return *GetData()->GetInt("rows"); }
        int         GetCols() { return *GetData()->GetInt("cols"); }
    };
}
