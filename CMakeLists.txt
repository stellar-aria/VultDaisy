cmake_minimum_required(VERSION 3.26)
cmake_policy(SET CMP0048 NEW)

# Fetch libDaisy
include(FetchContent)
FetchContent_Declare(daisy
  GIT_REPOSITORY https://github.com/stellar-aria/libDaisy
  GIT_TAG cmake-only
)
FetchContent_MakeAvailable(daisy)

# Our project declaration
project(vult_demo VERSION 0.0.1)

LIST(APPEND CMAKE_PROGRAM_PATH  "C:/Users/Kate/Developer/vult/") # only needed if vultc is not on your path
cmake_path(SET CMAKE_MODULE_LOCAL cmake)
cmake_path(ABSOLUTE_PATH CMAKE_MODULE_LOCAL)
list(APPEND CMAKE_MODULE_PATH ${CMAKE_MODULE_LOCAL})

set(FIRMWARE_NAME vult_demo)
set(FIRMWARE_SOURCES
  ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
)

# DaisyProject.cmake uses FIRMWARE_NAME and FIRMWARE_SOURCES to build a target called ${FIRMWARE_NAME}
include(DaisyProject)

target_include_directories(${FIRMWARE_NAME} PUBLIC include)

set_target_properties(${FIRMWARE_NAME} PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED YES
)
target_compile_options(${FIRMWARE_NAME} PRIVATE -Wno-volatile)

find_package(Vult REQUIRED)

target_link_libraries(${FIRMWARE_NAME} PRIVATE vult)

target_vult_sources(${FIRMWARE_NAME}
  src/phasedist.vult
)
