cmake_minimum_required(VERSION 3.17)
project(SetType)

set(CMAKE_CXX_STANDARD 14)

add_executable(Basic_SetType_Test SetType.h SetDriver.cpp)
add_executable(SetType_Tests unit_tests/SetType_Tests.cpp SetType.h unit_tests/unit_util.h)

if (APPLE)
    set_property(TARGET Basic_SetType_Test PROPERTY CXX_STANDARD 11)
    set_property(TARGET SetType_Tests PROPERTY CXX_STANDARD 11)
endif()
