cmake_minimum_required(VERSION 3.5)
project(SQLiteDAO LANGUAGES C CXX)

find_package(
  Python3 3.9
  REQUIRED
  COMPONENTS Interpreter
)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/dependencies.cmake)

add_subdirectory(daolib)

include(${CMAKE_CURRENT_LIST_DIR}/cmake/generate_or_update_dao/function_generate_or_update_dao.cmake)
