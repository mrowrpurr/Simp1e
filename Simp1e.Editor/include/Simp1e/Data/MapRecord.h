#pragma once

#include <Simp1e/Data/Record.h>
#include <Simp1e/Data/RecordData.h>
#include <string_format.h>

#include "MapGridCell.h"

namespace Simp1e::Data {

    class MapRecord {
        Data::Record* _record;

    public:
        MapRecord(Data::Record* record) : _record(record) {}

        Data::Record*     GetRecord() { return _record; }
        Data::RecordData* GetData() { return _record->GetData(); }

        const char* GetID() { return _record->GetFullIdentifier(); }

        int  GetRows() { return *GetData()->GetInt("rows"); }
        int  GetCols() { return *GetData()->GetInt("cols"); }
        bool HasContent() {
            auto hasMap = GetData()->HasMap("grid");
            //
            return GetData()->HasMap("grid");
        }

        std::optional<MapGridCell> GetCellAt(int x, int y) {
            auto cellData = GetData()->GetMap(string_format("grid.{}.{}", x, y).c_str());
            if (cellData.has_value()) return MapGridCell{std::move(*cellData)};
            else return {};
        }
    };
}
