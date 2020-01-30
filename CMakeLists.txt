cmake_minimum_required(VERSION 3.8)

project(ted LANGUAGES C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(Sources
	ted.c
    )

add_executable(ted ${Sources})
