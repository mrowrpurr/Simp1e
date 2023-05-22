#pragma once

#include <Simp1e/Data/DataStore.h>
#include <Simp1e/Data/Record.h>
#include <Simp1e/Data/RecordData.h>

#include <filesystem>
#include <memory>
#include <optional>

namespace Simp1e::Data {

    class MapGridCell {
        DataStore*                               _dataStore;
        std::unique_ptr<RecordData>              _cellRecordData;
        std::vector<std::unique_ptr<RecordData>> _layers;

    public:
        MapGridCell(DataStore* dataStore, std::unique_ptr<RecordData> cellRecordData)
            : _dataStore(dataStore), _cellRecordData(std::move(cellRecordData)) {
            if (HasLayers()) _layers = *_cellRecordData->GetObjectList("layers");
        }

        bool   HasLayers() const { return _cellRecordData->HasList("layers"); }
        size_t GetLayerCount() const { return _layers.size(); }
        std::unique_ptr<RecordData>&         GetLayer(size_t index) { return _layers[index]; }
        std::optional<std::filesystem::path> GetLayerImagePath(size_t index) {
            auto& layer = GetLayer(index);
            if (strcmp(layer->GetString("type")->c_str(), "image") != 0) return {};
            auto imageId = layer->GetString("imageId");
            if (!imageId) return {};
            auto* imageRecord = _dataStore->GetRecord((*imageId).c_str());
            if (!imageRecord) return {};
            auto* imageRecordData = imageRecord->GetData();
            if (!imageRecordData) return {};
            auto pathRootEnv = imageRecordData->GetString("root_env");
            if (!pathRootEnv) return {};
            // Right now, it's required. And it's an environment variable.
            // Get environment variable from pathRootEnv
            auto rootEnv = getenv((*pathRootEnv).c_str());
            if (!rootEnv) return {};
            auto filePath = imageRecordData->GetString("file");
            if (!filePath) return {};
            return std::filesystem::path(rootEnv) / (*filePath);
        }
    };
}
