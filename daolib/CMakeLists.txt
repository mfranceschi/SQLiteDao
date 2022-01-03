add_library(xyz STATIC 
    CommonQueries.cpp CommonQueries.hpp
    DAOLib.hpp DAOLib.cpp
    TheHeader.hpp
    Types.hpp
)
target_link_libraries(xyz PUBLIC SQLite_Cpp_Wrapper Magic_Enum String_Formatter)
target_include_directories(xyz PUBLIC ${CMAKE_CURRENT_LIST_DIR})

add_executable(xyztest main.cpp)
target_link_libraries(xyztest PRIVATE xyz )
