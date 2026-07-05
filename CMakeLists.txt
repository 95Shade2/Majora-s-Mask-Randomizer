cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0091 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

include(CMakeDependentOption)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

project(mm_rando VERSION "1.0.0" LANGUAGES C CXX CSharp)

include(CSharpUtilities)

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
  src/item.cpp
  src/kafei.cpp
  src/utils.cpp)

target_include_directories(randomizer PUBLIC
  include)

SETUP_EXECUTABLE(randomizer)

if(MSVC)
  target_link_options(randomizer PRIVATE "/STACK:16777216")
endif()

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
configure_files("MMR_GUI/item_tabs.json")
configure_files("MMR_GUI/Data/mm_bingo_standard.json")

configure_files("MMR_GUI/Data/item_slot_counts.json")

foreach(_gui_config debug release minsizerel relwithdebinfo)
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/MMR_GUI/item_tabs.json"
		"${PROJECT_BINARY_DIR}/${_gui_config}/item_tabs.json"
		COPYONLY)
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/MMR_GUI/Data/mm_bingo_standard.json"
		"${PROJECT_BINARY_DIR}/${_gui_config}/mm_bingo_standard.json"
		COPYONLY)
	configure_file(
		"${CMAKE_CURRENT_SOURCE_DIR}/MMR_GUI/Data/item_slot_counts.json"
		"${PROJECT_BINARY_DIR}/${_gui_config}/Data/item_slot_counts.json"
		COPYONLY)
endforeach()

file(MAKE_DIRECTORY "${PROJECT_BINARY_DIR}/MMR_GUI")
configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/MMR_GUI/app.manifest"
	"${PROJECT_BINARY_DIR}/MMR_GUI/app.manifest"
	COPYONLY)

add_executable(mm_rando_gui
	"MMR_GUI/App.config"

	"MMR_GUI/Program.cs"

	"MMR_GUI/Form1.cs"
	"MMR_GUI/Form1.Designer.cs"
	"MMR_GUI/Form1.resx"
	
	"MMR_GUI/LogicEditor.cs"
	"MMR_GUI/LogicEditor.Designer.cs"
	"MMR_GUI/LogicEditor.resx"
	
	"MMR_GUI/pmc.cs"
	"MMR_GUI/pmc.Designer.cs"
	"MMR_GUI/pmc.resx"

	"MMR_GUI/wallets_form.cs"
	"MMR_GUI/wallets_form.Designer.cs"
	"MMR_GUI/wallets_form.resx"
	
	"MMR_GUI/Item.cs"

	"MMR_GUI/UiTheme.cs"
	"MMR_GUI/ItemTabBuilder.cs"
	"MMR_GUI/TabCategoryIcons.cs"
	"MMR_GUI/ThemedTabControl.cs"
	"MMR_GUI/ThemeToggleSlider.cs"
	"MMR_GUI/PoolListView.cs"
	"MMR_GUI/PlandoUx.cs"
	"MMR_GUI/PlandoStrandedDialog.cs"
	"MMR_GUI/BingoTypes.cs"
	"MMR_GUI/BingoGoalList.cs"
	"MMR_GUI/BingoGoalValidator.cs"
	"MMR_GUI/BingoSrlGenerator.cs"
	"MMR_GUI/BingoCardGenerator.cs"
	"MMR_GUI/BingoCardExport.cs"
	"MMR_GUI/BingoCardDialog.cs"
	"MMR_GUI/BingoCardDialog.Designer.cs"
	"MMR_GUI/LogicUsefulness.cs"
	"MMR_GUI/ItemGrantCounts.cs"
	"MMR_GUI/LogicGraphInventoryPanel.cs"
	"MMR_GUI/ItemUsefulnessDialog.cs"
	"MMR_GUI/ItemUsefulnessDialog.Designer.cs"
	"MMR_GUI/LogicGraphDialog.cs"
	"MMR_GUI/LogicGraphCanvas.cs"
	"MMR_GUI/LogicGraphDialog.Designer.cs"
	"MMR_GUI/BingoGoalMark.cs"
	"MMR_GUI/BingoLinePopoutDialog.cs"
	"MMR_GUI/BingoLinePopoutDialog.Designer.cs"
	"MMR_GUI/BingoRerollLog.cs"
	"MMR_GUI/BingoRerollLogDialog.cs"
	"MMR_GUI/BingoRerollLogDialog.Designer.cs"
	"MMR_GUI/BingoParityHarness.cs"
	"MMR_GUI/PlandoStrandedDialog.Designer.cs"
	
	"MMR_GUI/CutscenesSelector.cs"
	"MMR_GUI/CutscenesSelector.Designer.cs"
	"MMR_GUI/CutscenesSelector.resx"
	
	"MMR_GUI/Properties/AssemblyInfo.cs"
	"MMR_GUI/Properties/Resources.Designer.cs"
	"MMR_GUI/Properties/Resources.resx"
	"MMR_GUI/Properties/Settings.Designer.cs"
	"MMR_GUI/Properties/Settings.settings"
)

csharp_set_windows_forms_properties(
	"MMR_GUI/Form1.cs"
	"MMR_GUI/Form1.Designer.cs"
	"MMR_GUI/Form1.resx"
	
	"MMR_GUI/LogicEditor.cs"
	"MMR_GUI/LogicEditor.Designer.cs"
	"MMR_GUI/LogicEditor.resx"
	
	"MMR_GUI/pmc.cs"
	"MMR_GUI/pmc.Designer.cs"
	"MMR_GUI/pmc.resx"

	"MMR_GUI/wallets_form.cs"
	"MMR_GUI/wallets_form.Designer.cs"
	"MMR_GUI/wallets_form.resx"
	
	"MMR_GUI/Item.cs"

	"MMR_GUI/UiTheme.cs"
	"MMR_GUI/ItemTabBuilder.cs"
	"MMR_GUI/TabCategoryIcons.cs"
	"MMR_GUI/ThemedTabControl.cs"
	"MMR_GUI/ThemeToggleSlider.cs"
	"MMR_GUI/PoolListView.cs"
	"MMR_GUI/PlandoUx.cs"
	"MMR_GUI/PlandoStrandedDialog.cs"
	"MMR_GUI/BingoTypes.cs"
	"MMR_GUI/BingoGoalList.cs"
	"MMR_GUI/BingoGoalValidator.cs"
	"MMR_GUI/BingoSrlGenerator.cs"
	"MMR_GUI/BingoCardGenerator.cs"
	"MMR_GUI/BingoCardExport.cs"
	"MMR_GUI/BingoCardDialog.cs"
	"MMR_GUI/BingoCardDialog.Designer.cs"
	"MMR_GUI/LogicUsefulness.cs"
	"MMR_GUI/ItemGrantCounts.cs"
	"MMR_GUI/LogicGraphInventoryPanel.cs"
	"MMR_GUI/ItemUsefulnessDialog.cs"
	"MMR_GUI/ItemUsefulnessDialog.Designer.cs"
	"MMR_GUI/LogicGraphDialog.cs"
	"MMR_GUI/LogicGraphCanvas.cs"
	"MMR_GUI/LogicGraphDialog.Designer.cs"
	"MMR_GUI/BingoGoalMark.cs"
	"MMR_GUI/BingoLinePopoutDialog.cs"
	"MMR_GUI/BingoLinePopoutDialog.Designer.cs"
	"MMR_GUI/BingoRerollLog.cs"
	"MMR_GUI/BingoRerollLogDialog.cs"
	"MMR_GUI/BingoRerollLogDialog.Designer.cs"
	"MMR_GUI/BingoParityHarness.cs"
	"MMR_GUI/PlandoStrandedDialog.Designer.cs"
	
	"MMR_GUI/CutscenesSelector.cs"
	"MMR_GUI/CutscenesSelector.Designer.cs"
	"MMR_GUI/CutscenesSelector.resx"
	
	"MMR_GUI/Properties/AssemblyInfo.cs"
	"MMR_GUI/Properties/Resources.Designer.cs"
	"MMR_GUI/Properties/Resources.resx"
	"MMR_GUI/Properties/Settings.Designer.cs"
	"MMR_GUI/Properties/Settings.settings"
)

set_target_properties(mm_rando_gui PROPERTIES WIN32_EXECUTABLE TRUE)

SETUP_EXECUTABLE(mm_rando_gui)

add_dependencies(mm_rando_gui randomizer)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT mm_rando_gui)

set_target_properties(mm_rando_gui PROPERTIES
  VS_DEBUGGER_WORKING_DIRECTORY "$<TARGET_FILE_DIR:mm_rando_gui>")

set_property(TARGET mm_rando_gui PROPERTY VS_DOTNET_TARGET_FRAMEWORK_VERSION "v4.6.1")

set_property(TARGET mm_rando_gui PROPERTY VS_GLOBAL_ApplicationManifest "MMR_GUI/app.manifest")

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
	"System.Web.Extensions"
	)