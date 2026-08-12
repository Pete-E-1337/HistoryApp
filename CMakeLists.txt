cmake_minimum_required(VERSION 3.6)
project(HistoryApp)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/)
include(svs_helpers)

svs_init(../Imports_v2 ../SVSLibrary)

#svs_restrict_platforms(vs2015_32)
svs_restrict_platforms(vs2015)

#svs_add_flag(WIN32 COMPILE "/wd4121")
#svs_add_flag(WIN32 COMPILE "/wd4127")
#svs_add_flag(WIN32 COMPILE "/wd4456")
#svs_add_flag(WIN32 COMPILE "/wd4458")
#svs_add_flag(WIN32 COMPILE "/wd4459")
#svs_add_flag(WIN32 COMPILE "/wd4505")

svs_version(1 0 0 0)

# Global vars ----------------------------------------------------------------------
# Set global cached library version variables. This way the first cmake in the chain
# can determine the version of the library that all other sub-projects use. If the
# global cached variable already exists then it doesn't get overridden, the existing
# value is used.
set(SVS_BOOST_VERSION "1.63.0" CACHE STRING "")
set(SVS_OPENCV_VERSION "3.2.0" CACHE STRING "")
set(SVS_CV_POSTFIX "320" CACHE STRING "")
set(SVS_INSTALL_DIR "Sentient Vision/WavebladeApp" CACHE STRING "")

# Configuration --------------------------------------------------------------------
svs_set_configurations(
#	ISENTRY_REWRITE_DEV
#	ISENTRY_REWRITE_FINAL
)

# Project ---------------------------------------------------------------------
svs_set_default_flags()

add_definitions(
	-DSVS_REMOVE_DEPRECATED_CODE
#	-DFILELOGGER_SINGLETON_DISABLE
)

if(WIN32)
	add_definitions(
#		-D__STDC_LIMIT_MACROS
#		-D_ENABLE_ATOMIC_ALIGNMENT_FIX
		-DSVS_RAPIDJSON_ERR_WCHAR=1
#		-DGLEW_STATIC
		-DWIN32_LEAN_AND_MEAN
	)
endif()

# Debug only flags
svs_add_flag(WIN32 COMPILE_DEBUG "-DSVS_THREAD_NAMING_ON")
svs_add_flag(WIN32 COMPILE_DEBUG "-D_DEBUG")

# Sources ---------------------------------------------------------------------
svs_src("Source Files\\"
		Source/AutonomousController
		Source/Settings
		Source/WavebladeDriver
		Source/HistoryApp
	)
	
svs_src("Source Files\\Gui\\"
#		Source/Gui/AboutDialog
#		Source/Gui/CalibrationFilesDialog
#        Source/Gui/GuiUtils
		Source/Gui/MainForm
#		Source/Gui/SerialSettingsDialog
	)
	
svs_src("Source Files\\Gui\\GeneratedFiles\\"
		Source/Gui/GeneratedFiles/History
	)

#svs_src("Source Files\\MathPlot\\"
#		Source/MathPlot/mathplot
#	)
	
#svs_auto_glob_src(
#   GLOB_RECURSE
#   ../iSentryBridge/Source/Settings/*.h
#   ../iSentryBridge/Source/Settings/*.cpp
#)
	
svs_auto_lib_src(
		Compiler/Breakpoint.h
		Compiler/Warnings.h
		Devices/SerialPort/AsyncSerial
		Devices/SerialPort/CallbackAsyncSerial
		Execution/BoostIoService
		Execution/FileLogger
		Execution/Mutex.h
		Execution/MutexBoost.cpp
		Logger/Log
		Math/MiscMath
		Memory/CircularBuffer
		Time/SVSdatetime
		Time/TimeHelper
		Std.h
		StringUtilities.h
	)
	
svs_lib_use(
#	FFmpegVideoPlayer
#	SpecialPaths
#	TinyXml2
)
   
# Windows version Info --------------------------------------------------------
if (WIN32)
	svs_create_win_versioninfo(
		versionInfoFiles
		NAME "History"
		ICON "${CMAKE_CURRENT_SOURCE_DIR}/Resources/Icon.ico"
	)

	svs_src(
		"Resource Files\\"
		${versionInfoFiles}
	)
endif()

# Imports ---------------------------------------------------------------------
#svs_import(DYNAMIC boost 1.60.0 system thread chrono filesystem date_time atomic regex bimap)
#svs_import(DYNAMIC boost 1.63.0 system thread chrono filesystem)
svs_import(HEADERS boost  ${SVS_BOOST_VERSION})
svs_import(HEADERS rapidjson 1.1.0)
#svs_import(HEADERS nod 0.4.1)
svs_import(DYNAMIC wxWidgets 3.0.2)
svs_import(DYNAMIC OpenCV ${SVS_OPENCV_VERSION} "opencv_world${SVS_CV_POSTFIX}")
#svs_import(HEADERS boost-enum 4.6)

# Add Subprojects -------------------------------------------------------------
#svs_add_subproject(../../ISentryShared)
#svs_prepend_to_target_lib(ISentryShared)
#include_directories(../../ISentryShared)
#
#svs_add_subproject(../VideoProcessorSettings)
#svs_prepend_to_target_lib(VidProcSettings)
#include_directories(../VideoProcessorSettings)

# Target ----------------------------------------------------------------------
svs_executable(History WIN32)
	
# Assets ----------------------------------------------------------------------
svs_add_asset("Resources/Icon.ico" "." RUNTIME)