#include <unordered_map>
#include <string>
#include <optional>
#include <fstream>
#include <format>
#include <sstream>
#include <vector>
#include <cstdint>
#include <bit>
#include "storage_engine.hpp"

    namespace {
        std::uint32_t nativeToLittleOrBack(std::uint32_t value) {
            if constexpr (std::endian::native == std::endian::little) {
                return value;
            } else {
                static_assert (
                    std::endian::native == std::endian::little ||
                    std::endian::native == std::endian::big,
                    "Unsupportd byte order"
                );

                return  ((value & 0x000000FFu) << 24) |
                        ((value & 0x0000FF00u) << 8)  |
                        ((value & 0x00FF0000u) >> 8)  |
                        ((value & 0xFF000000u) >> 24);
            }
        }
    }

    // append to log
    void StorageEngine::appendLog(char op, const std::string& key, const std::string& val) {
        std::uint32_t keyLength = static_cast<std::uint32_t>(key.size());
        std::uint32_t valLength = static_cast<std::uint32_t>(val.size());

        std::uint32_t storedKeyLength = nativeToLittleOrBack(keyLength);
        std::uint32_t storedValLength = nativeToLittleOrBack(valLength);

        std::ofstream file(
            "data/data.log",
            std::ios::binary | std::ios::app
        );

        if (!file) {
            throw std::runtime_error("Failed to open log file");
        }

        // Writing the operation
        file.write(&op, 1);

        // Writing key and value lengths
        file.write(
            reinterpret_cast<const char*>(&storedKeyLength),
            sizeof(keyLength)
        );
        file.write(
            reinterpret_cast<const char*>(&storedValLength),
            sizeof(valLength)
        );

        // Writing the actual key and value data
        file.write(key.data(), keyLength);
        file.write(val.data(), valLength);

        file.flush();
        if (!file) {
            throw std::runtime_error("Failed to write log record");
        }

    }

    StorageEngine::StorageEngine() {
        std::ifstream file("data/data.log", std::ios::binary);
        
        if (!file) {
            throw std::runtime_error("Could not open log");
        }

        while (true) {
            char op;

            if (!file.read(&op, 1)) {
            // EOF before a new record is the normal stopping point.
            if (file.eof() && !file.bad()) {
                break;
            }
            throw std::runtime_error("Could not read operation");
            }


            std::uint32_t keyLength;
            std::uint32_t valLength;

            if (!file.read(reinterpret_cast<char*>(&keyLength),
                       sizeof(keyLength)) ||
            !file.read(reinterpret_cast<char*>(&valLength),
                       sizeof(valLength))) {
            throw std::runtime_error("Incomplete record header");
            }

            keyLength = nativeToLittleOrBack(keyLength);
            valLength = nativeToLittleOrBack(valLength);

            // Add other run_time error checkers later like op check

            std::string key(keyLength, '\0');
            std::string val(valLength, '\0');

            // Add incomplete record data later
            file.read(key.data(), keyLength);
            file.read(val.data(), valLength);

            if (op == 'P') {
                dataMap[key] = val;
            } else {
                dataMap.erase(key);
            }
        }

    }

    void StorageEngine::put(const std::string& key, const std::string& val) {
        dataMap[key] = val;
        appendLog('P', key, val);
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
            appendLog('R', key, "");
            std::string val = dataMap.at(key);
            dataMap.erase(key);
            return val;
        } else {
            return std::nullopt;
        }
    }