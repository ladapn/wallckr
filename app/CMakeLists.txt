# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(wallckr)

target_sources(app PRIVATE src/main.cpp)

include_directories(lib/Robot)
include_directories(lib/Communication)
include_directories(lib/Control)
include_directories(lib/Motion)
include_directories(lib/Sensing)

add_subdirectory(lib/Communication)
add_subdirectory(lib/Motion)
add_subdirectory(lib/Control)
add_subdirectory(lib/Sensing)
