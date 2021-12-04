set(SQLITE_ZIP_PATH ${CMAKE_CURRENT_LIST_DIR}/sqlite-amalgamation-3370000.zip)
set(SQLITE_UNZIPPED_PATH ${CMAKE_CURRENT_BINARY_DIR}/sqlite-amalgamation-3370000)

if(NOT EXISTS ${SQLITE_UNZIPPED_PATH})
  execute_process(
    COMMAND 
      ${CMAKE_COMMAND} -E tar xf ${SQLITE_ZIP_PATH}
    WORKING_DIRECTORY
      ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()
file(GLOB SQLITE_SOURCES ${SQLITE_UNZIPPED_PATH}/*)

# Define static library SQLite_Unzipped which is just a built version of the original SQLite library.
add_library(SQLite_Unzipped STATIC ${SQLITE_SOURCES})
target_include_directories(SQLite_Unzipped 
  PRIVATE
#     ${SQLITE_UNZIPPED_PATH}
)

# Define interface SQLite that we actually use in the project.
add_library(SQLite INTERFACE)
target_include_directories(SQLite 
  INTERFACE 
    ${CMAKE_CURRENT_LIST_DIR}
)
target_link_libraries(SQLite INTERFACE SQLite_Unzipped)
