include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
    FIND_PACKAGE_ARGS NAMES GTest
)


set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_cases/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_cases/)

add_executable(
    ${PROJECT_NAME}_tests
    test_generator.cpp
)

target_link_libraries(
    ${PROJECT_NAME}_tests
    GTest::gtest_main
)

target_compile_features(${PROJECT_NAME}_tests PRIVATE cxx_std_23)

add_dependencies(${PROJECT_NAME}_tests generator)

include(GoogleTest)

gtest_discover_tests(${PROJECT_NAME}_tests)
