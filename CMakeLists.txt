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
	  file(CREATE_LINK "${sabs}" "${PROJECT_BINARY_DIR}/release/${s}" SYMBOLIC)
      file(CREATE_LINK "${sabs}" "${PROJECT_BINARY_DIR}/debug/${s}" SYMBOLIC)
      file(CREATE_LINK "${sabs}" "${PROJECT_BINARY_DIR}/minsizerel/${s}" SYMBOLIC)
      file(CREATE_LINK "${sabs}" "${PROJECT_BINARY_DIR}/relwithdebinfo/${s}" SYMBOLIC)
	  else()
	    message(ERROR "can't call SYMLINK_FILES with an absolute path")
    endif()
  endforeach()
endfunction()

macro(configure_files srcDir)
	if(NOT IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}")
		configure_file(
				"${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}"
				${PROJECT_BINARY_DIR}/release/${srcDir}
				COPYONLY)
		configure_file(
				"${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}"
				${PROJECT_BINARY_DIR}/debug/${srcDir}
				COPYONLY)
		configure_file(
				"${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}"
				${PROJECT_BINARY_DIR}/minsizerel/${srcDir}
				COPYONLY)
		configure_file(
				"${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}"
				${PROJECT_BINARY_DIR}/relwithdebinfo/${srcDir}
				COPYONLY)
	else()
		file(GLOB templateFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}" "${CMAKE_CURRENT_SOURCE_DIR}/${srcDir}/*")
		message(STATUS "template files: ${templateFiles}")
		foreach(templateFile ${templateFiles})
			set(srcTemplatePath ${srcDir}/${templateFile})
			if(NOT IS_DIRECTORY ${srcTemplatePath})
				message(STATUS "Configuring file ${templateFile}")
				configure_file(
						${srcTemplatePath}
						${PROJECT_BINARY_DIR}/release/${srcTemplatePath}
						COPYONLY)
				configure_file(
						${srcTemplatePath}
						${PROJECT_BINARY_DIR}/debug/${srcTemplatePath}
						COPYONLY)
				configure_file(
						${srcTemplatePath}
						${PROJECT_BINARY_DIR}/minsizerel/${srcTemplatePath}
						COPYONLY)
				configure_file(
						${srcTemplatePath}
						${PROJECT_BINARY_DIR}/relwithdebinfo/${srcTemplatePath}
						COPYONLY)
			endif(NOT IS_DIRECTORY ${srcTemplatePath})
		endforeach(templateFile)
	endif()
endmacro(configure_files)

function(SETUP_EXECUTABLE name)
  set_target_properties("${name}"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BINARY_DIR}/release")

  set_target_properties("${name}"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BINARY_DIR}/debug")

  set_target_properties("${name}"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${PROJECT_BINARY_DIR}/minsizerel")

  set_target_properties("${name}"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBUGINFO "${PROJECT_BINARY_DIR}/relwithdebuginfo")
endfunction()

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/release")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/debug")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/minsizerel")
file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/relwithdebinfo")

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

SETUP_EXECUTABLE(randomizer)

add_executable(ndec
  ndec/ndec.c
  )

SETUP_EXECUTABLE(ndec)

configure_files("_create-rom.bat")
configure_files("_create-roms.bat")
configure_files("_create-roms")
configure_files("cutscenes")
configure_files("files")
configure_files("logic")
configure_files("presets")

project(mm_rando_gui VERSION "1.0.0" LANGUAGES CSharp)

include(CSharpUtilities)

add_executable(mm_rando_gui
	"Majora's Mask Randomizer GUI/App.config"

	"Majora's Mask Randomizer GUI/Program.cs"

	"Majora's Mask Randomizer GUI/Form1.cs"
	"Majora's Mask Randomizer GUI/Form1.Designer.cs"
	"Majora's Mask Randomizer GUI/Form1.resx"
	
	"Majora's Mask Randomizer GUI/pmc.cs"
	"Majora's Mask Randomizer GUI/pmc.Designer.cs"
	"Majora's Mask Randomizer GUI/pmc.resx"

	"Majora's Mask Randomizer GUI/wallets_form.cs"
	"Majora's Mask Randomizer GUI/wallets_form.Designer.cs"
	"Majora's Mask Randomizer GUI/wallets_form.resx"
	
	"Majora's Mask Randomizer GUI/Properties/AssemblyInfo.cs"
	"Majora's Mask Randomizer GUI/Properties/Resources.Designer.cs"
	"Majora's Mask Randomizer GUI/Properties/Resources.resx"
	"Majora's Mask Randomizer GUI/Properties/Settings.Designer.cs"
	"Majora's Mask Randomizer GUI/Properties/Settings.settings"
)

csharp_set_windows_forms_properties(
	"Majora's Mask Randomizer GUI/Form1.cs"
	"Majora's Mask Randomizer GUI/Form1.Designer.cs"
	"Majora's Mask Randomizer GUI/Form1.resx"
	
	"Majora's Mask Randomizer GUI/pmc.cs"
	"Majora's Mask Randomizer GUI/pmc.Designer.cs"
	"Majora's Mask Randomizer GUI/pmc.resx"

	"Majora's Mask Randomizer GUI/wallets_form.cs"
	"Majora's Mask Randomizer GUI/wallets_form.Designer.cs"
	"Majora's Mask Randomizer GUI/wallets_form.resx"
	
	"Majora's Mask Randomizer GUI/Properties/AssemblyInfo.cs"
	"Majora's Mask Randomizer GUI/Properties/Resources.Designer.cs"
	"Majora's Mask Randomizer GUI/Properties/Resources.resx"
	"Majora's Mask Randomizer GUI/Properties/Settings.Designer.cs"
	"Majora's Mask Randomizer GUI/Properties/Settings.settings"
)

set_target_properties(mm_rando_gui PROPERTIES WIN32_EXECUTABLE TRUE)

SETUP_EXECUTABLE(mm_rando_gui)

set_property(TARGET mm_rando_gui PROPERTY VS_DOTNET_REFERENCES
	"System"
	"System.Core"
	"System.Xml.Linq"
	"System.Data.DataSetExtensions"
	"Microsoft.CSharp"
	"System.Data"
	"System.Deployment"
	"System.Drawing"
	"System.Net.Http"
	"System.Windows.Forms"
	"System.Xml"
	)