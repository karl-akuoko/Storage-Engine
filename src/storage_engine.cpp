#include <unordered_map>
#include <string>
#include <optional>
#include "storage_engine.hpp"

    void StorageEngine::put(const std::string& key, const std::string& val) {
        dataMap[key] = val;
    }

    std::optional<std::string> StorageEngine::get(const std::string& key) const {
        if (dataMap.contains(key)) {
            return dataMap.at(key);
        } else {
            return std::nullopt;
        }
    }

    std::optional<std::string> StorageEngine::remove(const std::string& key) {
        if (dataMap.contains(key)) {
            std::string val = dataMap.at(key);
            dataMap.erase(key);
            return val;
        } else {
            return std::nullopt;
        }
    }