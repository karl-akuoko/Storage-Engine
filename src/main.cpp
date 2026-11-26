#include <iostream>
#include "storage_engine.hpp"

int main() {
    auto check = [](bool passed, const char* description) {
    std::cout << (passed ? "PASS: " : "FAIL: ")
              << description << '\n';
};

    // Write and test normal operations.
    {
        StorageEngine engine;

        engine.put("test_name", "Karl");
        check(engine.get("test_name") == "Karl", "Put and get");

        engine.put("test_name", "Updated");
        check(engine.get("test_name") == "Updated", "Overwrite");

        engine.put("test_empty", "");
        check(engine.get("test_empty") == "", "Empty value");

        engine.put("test text key", "Hello world\nSecond line");
        check(
            engine.get("test text key") == "Hello world\nSecond line",
            "Spaces and newlines"
        );

        engine.put("test_deleted", "Delete me");
        check(
            engine.remove("test_deleted") == "Delete me",
            "Remove returns old value"
        );
        check(!engine.get("test_deleted"), "Deleted key is absent");
        check(!engine.remove("test_deleted"), "Remove missing key");
    }

    // A new instance must recover the state from the log.
    {
        StorageEngine recovered;

        check(
            recovered.get("test_name") == "Updated",
            "Overwrite survives reopening"
        );
        check(
            recovered.get("test_empty") == "",
            "Empty value survives reopening"
        );
        check(
            recovered.get("test text key") == "Hello world\nSecond line",
            "Text survives reopening"
        );
        check(
            !recovered.get("test_deleted"),
            "Deletion survives reopening"
        );
    }
}