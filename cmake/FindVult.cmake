include(FindPackageHandleStandardArgs)

find_program(Vult_EXECUTABLE NAMES vultc)

find_package_handle_standard_args(Vult REQUIRED_VARS Vult_EXECUTABLE)

if(Vult_FOUND)
  mark_as_advanced(Vult_EXECUTABLE)
endif()

if(Vult_FOUND AND NOT TARGET Vult::Vult)
  add_executable(Vult::Vult IMPORTED)
  set_property(TARGET Vult::Vult PROPERTY IMPORTED_LOCATION ${Vult_EXECUTABLE})

  cmake_path(GET Vult_EXECUTABLE PARENT_PATH Vult_FOLDER)
  add_library(vult ${Vult_FOLDER}/runtime/vultin.cpp)
  target_include_directories(vult PUBLIC ${Vult_FOLDER}/runtime)

  function(target_vult_sources TARGET)
    foreach(SOURCE IN LISTS ARGN)
      cmake_path(GET SOURCE STEM SOURCE_STEM)
      cmake_path(GET SOURCE PARENT_PATH SOURCE_PARENT)
      cmake_path(SET SOURCE_NO_EXT ${SOURCE_PARENT}/${SOURCE_STEM})

      cmake_path(ABSOLUTE_PATH SOURCE OUTPUT_VARIABLE SOURCE_ABSOLUTE)

      list(APPEND OUTPUTS ${SOURCE_NO_EXT}.cpp )
      list(APPEND OUTPUT_DIRS ${CMAKE_CURRENT_BINARY_DIR}/${SOURCE_PARENT})
      add_custom_command(
        MAIN_DEPENDENCY
          ${SOURCE}
        OUTPUT
          ${SOURCE_NO_EXT}.cpp
          ${SOURCE_NO_EXT}.h
        COMMAND
          ${Vult_EXECUTABLE} -ccode ${SOURCE_ABSOLUTE} -template performance -o ${SOURCE_NO_EXT}
        COMMENT "Transpiling Vult source ${SOURCE}"
      )
    endforeach()
    target_sources(${TARGET} PRIVATE ${OUTPUTS})
    target_include_directories(${TARGET} PUBLIC ${OUTPUT_DIRS})
  endfunction()
endif()
