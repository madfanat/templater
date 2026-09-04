include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/releases/download/v1.16.0/googletest-1.16.0.tar.gz
  FIND_PACKAGE_ARGS NAMES GTest
)

if(MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

FetchContent_MakeAvailable(googletest)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test_cases/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/test_cases/)

add_executable(
    templater_tests
    tests.cc
)

target_link_libraries(
    templater_tests
    PRIVATE
        GTest::gtest_main
)

target_compile_features(templater_tests PRIVATE cxx_std_23)

add_dependencies(templater_tests templater)

include(GoogleTest)

gtest_discover_tests(templater_tests)
