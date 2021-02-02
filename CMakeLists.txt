cmake_minimum_required(VERSION 3.15)
if(CMAKE_GENERATOR MATCHES "Visual Studio")
  # so MSVC will compile and link correctly
  cmake_policy(SET CMP0091 NEW)
else()
  cmake_minimum_required(VERSION 3.15)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(CMakeDependentOption)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

project(mm_rando VERSION "1.0.0" LANGUAGES C CXX)

include(GNUInstallDirs)

# build flags
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
endif()

if(NOT CMAKE_CONFIGURATION_TYPES)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

##
# @brief Symbolically links a list of files to build directory
# @param ARGN files to link. Must be relative paths!
function(SYMLINK_FILES)
  # make absolute paths
  foreach(s IN LISTS ARGN)
    if(NOT IS_ABSOLUTE "${s}")
      get_filename_component(sabs "${s}" ABSOLUTE)
	    file(CREATE_LINK "${sabs}" "${CMAKE_CURRENT_BINARY_DIR}/release/${s}" SYMBOLIC)
      file(CREATE_LINK "${sabs}" "${CMAKE_CURRENT_BINARY_DIR}/debug/${s}" SYMBOLIC)
      file(CREATE_LINK "${sabs}" "${CMAKE_CURRENT_BINARY_DIR}/minsizerel/${s}" SYMBOLIC)
      file(CREATE_LINK "${sabs}" "${CMAKE_CURRENT_BINARY_DIR}/relwithdebinfo/${s}" SYMBOLIC)
	  else()
	    message(ERROR "can't call SYMLINK_FILES with an absolute path")
    endif()
  endforeach()
endfunction()

file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/release")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/debug")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/minsizerel")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/relwithdebinfo")

add_executable(randomizer
  src/main.cpp
  src/logging.cpp
  src/color.cpp
  src/io.cpp
  src/rom.cpp
  src/inventory.cpp
  src/time.cpp
  src/item.cpp
  src/kafei.cpp
  src/utils.cpp)

target_include_directories(randomizer PUBLIC
  include)

set_target_properties(randomizer
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/release")

set_target_properties(randomizer
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/debug")

set_target_properties(randomizer
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_BINARY_DIR}/minsizerel")

set_target_properties(randomizer
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBUGINFO "${PROJECT_BINARY_DIR}/relwithdebuginfo")

add_executable(ndec
  ndec/ndec.c
  )

set_target_properties(ndec
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/release")

set_target_properties(ndec
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/debug")

set_target_properties(ndec
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_BINARY_DIR}/minsizerel")

set_target_properties(ndec
  PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBUGINFO "${PROJECT_BINARY_DIR}/relwithdebuginfo")

# symlink important directories
SYMLINK_FILES("_create-rom.bat"
  "_create-roms.bat"
  "_create-roms"
  "cutscenes"
  "files"
  "logic"
  "presets")
