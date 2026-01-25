cmake_minimum_required(VERSION 3.15)
project(api2spec-fixture-crow)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  Crow
  GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
  GIT_TAG v1.0+5
)
FetchContent_MakeAvailable(Crow)

add_executable(server main.cpp)
target_link_libraries(server Crow::Crow)

# Testing
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    enable_testing()

    FetchContent_Declare(
      googletest
      GIT_REPOSITORY https://github.com/google/googletest.git
      GIT_TAG v1.14.0
    )
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    add_executable(api_test tests/api_test.cpp)
    target_link_libraries(api_test Crow::Crow GTest::gtest_main)
    target_include_directories(api_test PRIVATE ${CMAKE_SOURCE_DIR})

    include(GoogleTest)
    gtest_discover_tests(api_test)
endif()
