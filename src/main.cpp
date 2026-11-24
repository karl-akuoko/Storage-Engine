#include <iostream>
#include "storage_engine.hpp"

int main() {
    StorageEngine engine;
    engine.put("name", "Karl");
    engine.put("age", "20");
    std::cout << engine.get("name").value_or("Not found") << '\n';

    engine.remove("name");
    std::cout << engine.get("name").value_or("Not found") << '\n';
}