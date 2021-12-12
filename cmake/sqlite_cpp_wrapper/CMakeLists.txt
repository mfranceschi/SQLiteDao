# Defines the library SQLite_Cpp_Wrapper.
# If we change the CPP wrapper then no CMake code should change except for this file.

include(FetchContent)

# set(SQLITECPP_INTERNAL_SQLITE OFF)
FetchContent_Declare(
    sqlitecpp_fetched 
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git 
    GIT_TAG 3.1.1
    GIT_CONFIG core.autocrlf=false 
)
FetchContent_MakeAvailable(sqlitecpp_fetched)

# Define interface SQLite that we actually use in the project.
add_library(SQLite_Cpp_Wrapper ALIAS SQLiteCpp)
