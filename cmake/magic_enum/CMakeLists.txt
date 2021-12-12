
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
