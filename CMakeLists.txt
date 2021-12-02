cmake_minimum_required(VERSION 3.0)
project(SQLiteDAO LANGUAGES CXX)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

add_executable(test)
target_sources(test PRIVATE test.cpp)
