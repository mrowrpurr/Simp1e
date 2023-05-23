#pragma once

#include <Simp1e/Data/Record.h>

#include <filesystem>
#include <optional>

namespace Simp1e::Data::Types {

    class ImageRecord {
        Record* _record;

        RecordData* GetData() { return _record->GetData(); }

    public:
        ImageRecord(Record* record) : _record(record) {}

        std::optional<std::string> GetFileName() {
            auto data = GetData();
            if (!data) return {};
            return data->GetString("file");
        }

        std::optional<std::string> GetRootPath() {
            auto data = GetData();
            if (!data) return {};
            auto rootEnvironmentVariable = data->GetString("root_env");
            if (rootEnvironmentVariable)
                return std::filesystem::path{std::getenv(rootEnvironmentVariable.value().c_str())}
                    .string();
            else return data->GetString("root");
        }

        std::optional<std::string> GetImagePath() {
            auto data = GetData();
            if (!data) return {};
            auto rootPath = GetRootPath();
            if (!rootPath) return {};
            auto fileName = GetFileName();
            if (!fileName) return {};
            return std::filesystem::path{rootPath.value() + "/" + fileName.value()}.string();
        }
    };
}
