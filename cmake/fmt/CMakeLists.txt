include(FetchContent)

FetchContent_Declare(
    fmt_fetched 
    GIT_REPOSITORY https://github.com/fmtlib/fmt.git 
    GIT_TAG 8.0.1
    GIT_CONFIG core.autocrlf=false 
)
FetchContent_MakeAvailable(fmt_fetched)

# Define interface SQLite that we actually use in the project.
add_library(String_Formatter ALIAS fmt)
