cmake_minimum_required(VERSION 3.10)
project(untitled C)

set(CMAKE_C_STANDARD 23)

add_executable(sysmon main.c
        memory.c
        cpu.c
        process.c
        utils.c
        include/sysmon.h
        include/render.h
        render.c
        ${CMAKE_CURRENT_SOURCE_DIR}/external/termbox2/termbox2.o)

target_include_directories(sysmon
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)