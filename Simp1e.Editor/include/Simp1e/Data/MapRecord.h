#pragma once

#include <Simp1e/Data/DataStore.h>
#include <Simp1e/Data/Record.h>
#include <Simp1e/Data/RecordData.h>
#include <string_format.h>

#include "MapGridCell.h"

namespace Simp1e::Data {

    class MapRecord {
        DataStore* _dataStore;
        Record*    _record;

    public:
        MapRecord(DataStore* dataStore, Record* record) : _dataStore(dataStore), _record(record) {}

        Data::Record*     GetRecord() { return _record; }
        Data::RecordData* GetData() { return _record->GetData(); }

        const char* GetID() { return _record->GetFullIdentifier(); }

        int  GetRows() { return *GetData()->GetInt("rows"); }
        int  GetCols() { return *GetData()->GetInt("cols"); }
        bool HasContent() { return GetData()->HasMap("grid"); }

        std::optional<MapGridCell> GetCellAt(int x, int y) {
            auto cellData = GetData()->GetMap(string_format("grid.{}.{}", x, y).c_str());
            if (cellData.has_value()) return MapGridCell{_dataStore, std::move(*cellData)};
            else return {};
        }
    };
}
