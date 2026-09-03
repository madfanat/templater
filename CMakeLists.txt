cmake_minimum_required(VERSION 3.20)

project(templater VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_executable(
    ${PROJECT_NAME}
    src/main.cc
    src/parser.cc
)

add_subdirectory(tests)
