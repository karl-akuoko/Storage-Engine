cmake_minimum_required(VERSION 3.20)

project(StorageEngine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(storage_engine src/main.cpp)

target_include_directories(storage_engine PRIVATE include)

target_compile_options(storage_engine PRIVATE
    -Wall -Wextra -Wpedantic
)
