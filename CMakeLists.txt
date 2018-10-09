cmake_minimum_required(VERSION 3.12)
project(C_Programs C)

set(CMAKE_C_STANDARD 11)

add_executable(C_Programs
        calculator.c)

target_link_libraries(C_Programs m)
