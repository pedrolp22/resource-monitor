cmake_minimum_required(VERSION 4.1)
project(untitled C)

set(CMAKE_C_STANDARD 23)

add_executable(sysmon main.c
        memory.c
        cpu.c
        process.c
        utils.c
        include/sysmon.h)

target_include_directories(sysmon
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)