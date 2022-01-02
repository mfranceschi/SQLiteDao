generate_or_update_dao(
  YAML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/a.yaml
  USER_DAO_LIB_NAME MyLibA
)

add_executable(test_1_exec)
target_sources(test_1_exec PRIVATE main.cpp)
target_link_libraries(test_1_exec PRIVATE MyLibA)
