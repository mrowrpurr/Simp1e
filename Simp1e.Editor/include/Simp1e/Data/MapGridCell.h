#pragma once

#include <Simp1e/Data/Record.h>
#include <Simp1e/Data/RecordData.h>

#include <memory>

namespace Simp1e::Data {

    class MapGridCell {
        std::unique_ptr<RecordData> _cellRecordData;

    public:
        MapGridCell(std::unique_ptr<RecordData> cellRecordData)
            : _cellRecordData(std::move(cellRecordData)) {}
    };
}
