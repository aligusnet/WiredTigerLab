cmake_minimum_required(VERSION 3.10)

project(WiredTigerLab)

include(ExternalProject)
include(wiredtiger.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/install)

add_wiredtiger(wiredtiger "mongodb-5.3.0" ${INSTALL_DIR})

add_executable(wtlab "src/main.cpp" "src/wtapi.ixx" )
target_include_directories(wtlab PUBLIC ${INSTALL_DIR}/include)
target_link_libraries(wtlab ${wiredtiger})
