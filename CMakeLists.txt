# Copyright (c) 2026 Agile Robots SE
# SPDX-License-Identifier: MIT
cmake_minimum_required(VERSION 3.10)
project(diana_api_examples_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(DIANA_SDK_DIR "${CMAKE_SOURCE_DIR}/third_party/diana_sdk_cpp" CACHE PATH "Path to Diana C/C++ SDK")

find_package(Threads REQUIRED)

find_path(DIANA_INCLUDE_DIR
    NAMES DianaAPI.h
    PATHS
        "${DIANA_SDK_DIR}/include"
        "${DIANA_SDK_DIR}"
    NO_DEFAULT_PATH)

find_library(DIANA_API_LIBRARY
    NAMES DianaApi libDianaApi
    PATHS
        "${DIANA_SDK_DIR}/lib"
        "${DIANA_SDK_DIR}/bin_linux"
        "${DIANA_SDK_DIR}"
    NO_DEFAULT_PATH)

if(NOT DIANA_INCLUDE_DIR)
    message(FATAL_ERROR
        "DianaAPI.h not found. Copy the official SDK to third_party/diana_sdk_cpp, "
        "or configure with -DDIANA_SDK_DIR=/path/to/diana_sdk_cpp")
endif()

if(NOT DIANA_API_LIBRARY)
    message(FATAL_ERROR
        "libDianaApi not found. Copy the official SDK to third_party/diana_sdk_cpp, "
        "or configure with -DDIANA_SDK_DIR=/path/to/diana_sdk_cpp")
endif()

add_library(example_common STATIC
    common/example_common.cpp
    robots/robot_registry.cpp)
target_include_directories(example_common PUBLIC
    "${CMAKE_SOURCE_DIR}/common"
    "${CMAKE_SOURCE_DIR}/robots"
    "${DIANA_INCLUDE_DIR}")
target_link_libraries(example_common PUBLIC "${DIANA_API_LIBRARY}" Threads::Threads)

function(add_diana_example target source_dir)
    add_executable(${target} "examples/${source_dir}/main.cpp")
    target_link_libraries(${target} PRIVATE example_common)
    set_target_properties(${target} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
endfunction()

add_diana_example(01_connection       01_connection)
add_diana_example(02_robot_state      02_robot_state)
add_diana_example(03_joint_motion     03_joint_motion)
add_diana_example(04_cartesian_motion 04_cartesian_motion)
add_diana_example(05_kinematics       05_kinematics)
add_diana_example(06_io               06_io)
add_diana_example(07_tcp_and_frames   07_tcp_and_frames)
add_diana_example(08_motion_limits    08_motion_limits)
add_diana_example(09_path_and_waypoints 09_path_and_waypoints)
add_diana_example(10_jog_motion       10_jog_motion)
add_diana_example(11_force_guarded_motion 11_force_guarded_motion)
add_diana_example(12_servoL_realtime  12_servoL_realtime)
