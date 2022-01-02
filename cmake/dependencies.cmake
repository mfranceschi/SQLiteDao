include(FetchContent)


##########################################
########## STRING FORMATTER ##############
########## Choice: fmt      ##############
##########################################

FetchContent_Declare(
    fmt_fetched 
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git 
    GIT_TAG 8.0.1
    GIT_CONFIG core.autocrlf=false 
)
FetchContent_MakeAvailable(fmt_fetched)

# Define interface SQLite that we actually use in the project.
add_library(String_Formatter ALIAS fmt)


##########################################
########## MAGIC ENUM       ##############
##########################################

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/magic_enum/magic_enum.hpp)
    file(DOWNLOAD 
        https://raw.githubusercontent.com/Neargye/magic_enum/v0.7.3/include/magic_enum.hpp
        ${CMAKE_CURRENT_BINARY_DIR}/magic_enum/magic_enum.hpp
    )
endif()

add_library(
  Magic_Enum INTERFACE
)
target_include_directories(Magic_Enum 
  INTERFACE 
    ${CMAKE_CURRENT_BINARY_DIR}/magic_enum
)


##########################################
########## SQLITE CPP WRAPPER   ##########
########## Choice: fmt          ##########
##########################################

FetchContent_Declare(
    sqlitecpp_fetched 
    GIT_REPOSITORY https://github.com/SRombauts/SQLiteCpp.git 
    GIT_TAG 3.1.1
    GIT_CONFIG core.autocrlf=false 
)
FetchContent_MakeAvailable(sqlitecpp_fetched)

# Define interface SQLite that we actually use in the project.
add_library(SQLite_Cpp_Wrapper ALIAS SQLiteCpp)
