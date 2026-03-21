cmake_minimum_required(VERSION 4.1)
project(untitled C)

set(CMAKE_C_STANDARD 23)

add_executable(sysmon main.c
        memory.c
        cpu.c
        process.c
        utils.c
        sysmon.h)
