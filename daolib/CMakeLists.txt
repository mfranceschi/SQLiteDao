add_library(xyz STATIC 
    CommonQueries.cpp CommonQueries.hpp
    DAOLib.hpp DAOLib.cpp
    Types.hpp
)
target_link_libraries(xyz PUBLIC SQLite_Cpp_Wrapper Magic_Enum String_Formatter)

add_executable(xyztest main.cpp)
target_link_libraries(xyztest PRIVATE xyz )
