cmake_minimum_required(VERSION 3.10)

project(WiredTigerLab)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(WT_PATH "c:/wiredtiger")

find_library(WT wiredtiger)

add_executable(wtlab "src/main.cpp")

target_include_directories(wtlab PUBLIC "${WT_PATH}/include")

target_link_libraries(wtlab ${WT})