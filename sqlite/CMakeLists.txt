include(FetchContent)
FetchContent_Declare(sqlite_fetched URL https://www.sqlite.org/2021/sqlite-amalgamation-3370000.zip)
FetchContent_MakeAvailable(sqlite_fetched)

# Define interface SQLite that we actually use in the project.
add_library(
  SQLite STATIC
  ${sqlite_fetched_SOURCE_DIR}/sqlite3.c
  ${sqlite_fetched_SOURCE_DIR}/sqlite3.h
)
target_include_directories(SQLite 
  PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
  PRIVATE
    ${sqlite_fetched_SOURCE_DIR}
)
