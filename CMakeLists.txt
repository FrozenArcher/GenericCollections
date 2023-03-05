cmake_minimum_required(VERSION 3.10)
project(generic_collections C)
set(CMAKE_C_STANDARD 99)

set (SRC_LIST arrays.c main.c)

add_executable(${PROJECT_NAME} ${SRC_LIST})