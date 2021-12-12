add_library(xyz STATIC 
    DAOLib.hpp DAOLib.cpp
)
target_link_libraries(xyz PUBLIC SQLite_Cpp_Wrapper Magic_Enum)

add_executable(xyztest main.cpp)
target_link_libraries(xyztest PRIVATE xyz )
