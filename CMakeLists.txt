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

add_executable(randomizer
  src/main.cpp
  src/error.cpp)
target_include_directories(randomizer PUBLIC
  include)
