add_library(xyz STATIC 
    # ConnectedDatabaseFactory.hpp
    DAOLib.hpp DAOLib.cpp
    # Database.hpp Database.cpp
    SQLiteError.hpp
)
target_link_libraries(xyz PUBLIC SQLite_Cpp_Wrapper)

add_executable(xyztest main.cpp)
target_link_libraries(xyztest PRIVATE xyz )
