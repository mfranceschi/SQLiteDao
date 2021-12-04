add_library(xyz STATIC 
    ConnectedDatabaseFactory.hpp
    DAOLib.hpp DAOLib.cpp
    Database.hpp Database.cpp
)
target_link_libraries(xyz PUBLIC SQLite)

add_executable(xyztest main.cpp)
target_link_libraries(xyztest PRIVATE xyz)
