add_library(xyz STATIC DAOLib.hpp DAOLib.cpp)
add_executable(xyztest main.cpp)
target_link_libraries(xyztest PRIVATE xyz SQLite)
