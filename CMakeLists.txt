cmake_minimum_required(VERSION 3.24)

project(templater LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(
    ${PROJECT_NAME}
    src/main.cc
    src/parser.cc
)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
