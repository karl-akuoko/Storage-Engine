#ifndef STORAGE_ENGINE_H
#define STORAGE_ENGINE_H

#include <unordered_map>
#include <string>
#include <optional>

class StorageEngine {
    private:
        // Key, Value store and size
        std::unordered_map<std::string, std::string> dataMap;

    public:
        void put(const std::string& key, const std::string& val);
        std::optional<std::string> get(const std::string& key) const;
        std::optional<std::string>  remove(const std::string& key); 
};
#endif
