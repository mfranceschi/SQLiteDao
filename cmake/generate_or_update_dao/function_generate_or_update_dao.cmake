
function(GENERATE_OR_UPDATE_DAO)
  cmake_parse_arguments(
    THIS_ARGS # prefix of output variables
    "" # booleans / options
    "USER_DAO_LIB_NAME;YAML_FILE" # mono-valued arguments
    "" # multi-valued arguments. TODO 'YAML_FILES' here
    ${ARGN} # arguments to parse
  )

  # We will write C++ files (among others) here.
  set(USER_DAO_LIB_OUTPUT_FOLDER ${CMAKE_CURRENT_BINARY_DIR}/${THIS_ARGS_USER_DAO_LIB_NAME})

  # Create a new library to which the user will link.
  add_library(${THIS_ARGS_USER_DAO_LIB_NAME} STATIC)
  set(USER_DAO_LIB_CPP_FILES )

  # One C++ module per YAML file.
  # Re-generate C++ code if the YAML is newer.
  get_filename_component(YAML_FILE_WE ${THIS_ARGS_YAML_FILE} NAME_WE)
  set(CPP_FILE_OF_CURRENT_YAML ${USER_DAO_LIB_OUTPUT_FOLDER}/${YAML_FILE_WE}.hpp)
  if (${THIS_ARGS_YAML_FILE} IS_NEWER_THAN ${CPP_FILE_OF_CURRENT_YAML})
    message("YAML is newer than C++ file, generating.")
    set(_COMMAND ${Python3_EXECUTABLE} ${SQLiteDAO_PYTHON_ENTRY_POINT} --yaml_file=${THIS_ARGS_YAML_FILE} --output_folder=${USER_DAO_LIB_OUTPUT_FOLDER})
    execute_process(
      COMMAND ${_COMMAND}
      COMMAND_ERROR_IS_FATAL ANY
    )
  endif()
  list(APPEND USER_DAO_LIB_CPP_FILES ${CPP_FILE_OF_CURRENT_YAML})

  # Last configs of the library.
  target_sources(${THIS_ARGS_USER_DAO_LIB_NAME} PRIVATE ${USER_DAO_LIB_CPP_FILES})
  target_link_libraries(${THIS_ARGS_USER_DAO_LIB_NAME} PUBLIC xyz)

endfunction()
