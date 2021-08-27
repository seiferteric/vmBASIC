# src/CMakeLists.txt
cmake_minimum_required(VERSION 3.10)

project(vmBASIC VERSION 0.1)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_STANDARD_REQUIRED True)

add_subdirectory(cpu) 

add_executable(vmBASIC "main.c")

target_link_libraries(vmBASIC PUBLIC cpu)

configure_file(version.h.in version.h)

target_include_directories(vmBASIC PUBLIC
                           "${PROJECT_BINARY_DIR}"
                           "${PROJECT_SOURCE_DIR}/cpu"
                           )

