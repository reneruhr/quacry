cmake_minimum_required(VERSION 3.5)

project(LLLTest)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(../../../kipod ${CMAKE_CURRENT_BINARY_DIR}/kipod_build)
include_directories(../../../kipod/include)
include_directories(../../../kipod/external_libs/)

file(GLOB catch2
    "../../../kipod/external_libs/catch2/*.hpp"
    "../../../kipod/external_libs/catch2/*.cpp"
)

add_executable(LLLTest
  ${catch2}
  LLLtest.cpp
)

target_link_libraries(LLLTest PUBLIC kipod)
