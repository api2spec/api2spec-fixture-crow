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
