cmake_minimum_required(VERSION 3.0)
project(SQLiteDAO LANGUAGES C CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sqlite)
add_subdirectory(daolib)
