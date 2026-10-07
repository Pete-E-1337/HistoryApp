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

#required for wxFont deprecation warning using wxWidgets 3.2.11
svs_add_flag(WIN32 COMPILE "/wd4996")
#required for "needs to have dll-interface to be used by clients" using wxWidgets 3.2.11
svs_add_flag(WIN32 COMPILE "/wd4251")

svs_version(1 0 0 0)

# Global vars ----------------------------------------------------------------------
# Set global cached library version variables. This way the first cmake in the chain
# can determine the version of the library that all other sub-projects use. If the
# global cached variable already exists then it doesn't get overridden, the existing
# value is used.
set(SVS_BOOST_VERSION "1.63.0" CACHE STRING "")
set(SVS_OPENCV_VERSION "3.2.0" CACHE STRING "")
set(SVS_CV_POSTFIX "320" CACHE STRING "")
set(SVS_INSTALL_DIR "Sentient/HistoryApp" CACHE STRING "")

# Configuration --------------------------------------------------------------------
svs_set_configurations(
#	ISENTRY_REWRITE_DEV
#	ISENTRY_REWRITE_FINAL
)

# Project ---------------------------------------------------------------------
svs_set_default_flags()





## --- CRITICAL FIX: STRIP WXUSINGDLL FROM AUTOMATIC VARIABLES ---
## This stops CMake from passing the DLL macro to the compiler and Resource Compiler
#if(wxWidgets_DEFINITIONS)
#    list(REMOVE_ITEM wxWidgets_DEFINITIONS "-DWXUSINGDLL" "WXUSINGDLL=1" "WXUSINGDLL")
#endif()





add_definitions(
	-DSVS_REMOVE_DEPRECATED_CODE
#	-DFILELOGGER_SINGLETON_DISABLE
	-D__WXMSW__
	-DNOPCH
	-DwxDEBUG_LEVEL=1
#	-DWXUSINGDLL=0
)

if(WIN32)
	add_definitions(
#		-D__STDC_LIMIT_MACROS
#		-D_ENABLE_ATOMIC_ALIGNMENT_FIX
		-DSVS_RAPIDJSON_ERR_WCHAR=1
#		-DGLEW_STATIC
		-DWIN32_LEAN_AND_MEAN
#-DwxWidgets_ROOT_DIR=/path/to/wxwidgets       - untested
	)
endif()

# Debug only flags
svs_add_flag(WIN32 COMPILE_DEBUG "-DSVS_THREAD_NAMING_ON")
svs_add_flag(WIN32 COMPILE_DEBUG "-D_DEBUG")

# Sources ---------------------------------------------------------------------
svs_src("Source Files\\"
#		Source/AutonomousController
		Source/Settings
#		Source/WavebladeDriver
		Source/HistoryApp
		Source/AppData
		Source/PenguinMaps
	)
	
svs_src("Source Files\\Gui\\"
#		Source/Gui/AboutDialog
#		Source/Gui/CalibrationFilesDialog
#        Source/Gui/GuiUtils
		Source/Gui/MainForm
		Source/Gui/TimelineGLCanvas
		Source/Gui/ImageDialog
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
#		Devices/SerialPort/AsyncSerial
#		Devices/SerialPort/CallbackAsyncSerial
		Execution/BoostIoService
		Execution/FileLogger
		Execution/Mutex.h
		Execution/MutexBoost.cpp
		Execution/Thread
		GUI/OpenGLHelper
		GUI/OpenGLFont
		GUI/Font
		FileIO/AsyncFileLoader
		Logger/Log
		Math/MiscMath
		Math/Matrix4.h
#		Memory/CircularBuffer
		Memory/MemoryPool
		Spatial/Angle
#		Spatial/Geometrics
#		Spatial/Line2D
		Spatial/Point2D
		Spatial/Vector2D
		Spatial/Vector3D
		Structures/DrawingPrimitives
		Structures/Colors
		Time/SVSdatetime
		Time/TimeHelper
		Std.h
		StringUtilities.h
		StringUtilities.inl
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




# Require C++14 or C++17 as preferred by wxWidgets 3.2+
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# --- CRITICAL FLAGS FOR STATIC LINKING ---
set(wxWidgets_USE_STATIC OFF)         # Forces CMake to calculate dynamic dependencies
set(wxWidgets_USE_UNICODE ON)         # Ensures it maps to Unicode characters

# Tell CMake where the main wxWidgets folder is
set(wxWidgets_ROOT_DIR "D:/Work/Projects/Imports_v2/wxWidgets/3.2.11")

# Crucial: Tell CMake exactly where the compiled library folder sits
# Look inside your wxWidgets/lib/ directory to see what your folder is named 
# (e.g., vc_x64_lib, vc_lib, gcc_x64_lib)
#set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib/vc14x_dll" CACHE PATH "" FORCE) 
set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib/vc14x_dll") 

set(wxWidgets_CONFIGURATION mswud) # The 'd' at the end forces Debug libraries

# Find wxWidgets package and required components (core and base are mandatory)
#find_package(wxWidgets REQUIRED COMPONENTS core base)
find_package(wxWidgets)

# Include wxWidgets configuration headers and definitions
include(${wxWidgets_USE_FILE})

# extra lib
svs_add_to_target_lib(wxmsw32ud_core.lib wxbase32ud.lib wxbase32ud_net.lib wxbase32ud_xml.lib wxmsw32ud_adv.lib wxmsw32ud_aui.lib wxmsw32ud_gl.lib wxmsw32ud_html.lib wxmsw32ud_media.lib wxmsw32ud_propgrid.lib wxmsw32ud_ribbon.lib wxmsw32ud_richtext.lib wxmsw32ud_stc.lib wxmsw32ud_webview.lib wxmsw32ud_xrc.lib)



# Imports ---------------------------------------------------------------------
#svs_import(DYNAMIC boost 1.60.0 system thread chrono filesystem date_time atomic regex bimap)
#svs_import(DYNAMIC boost 1.63.0 system thread chrono filesystem)
#svs_import(HEADERS boost-enum 4.6)
svs_import(HEADERS boost  ${SVS_BOOST_VERSION})
svs_import(HEADERS rapidjson 1.1.0)
#svs_import(HEADERS nod 0.4.1)
svs_import(DYNAMIC freeglut 2.8.1 freeglut)
#svs_import(DYNAMIC wxWidgets 3.0.2)
#svs_import(DYNAMIC wxWidgets 3.2.11)
#svs_import(HEADERS wxWidgets 3.2.11)
svs_import(DYNAMIC OpenCV ${SVS_OPENCV_VERSION} "opencv_world${SVS_CV_POSTFIX}")
svs_import(DYNAMIC glew 1.6.0 glew32)
svs_import(STATIC freetype 2.4.10 freetype2410)




## Manually define includes and libraries for CMake 3.6 fallback
#set(wxWidgets_ROOT_DIR "D:/Work/Projects/Imports_v2/wxWidgets/3.2.11/vs2015")
#
## Replace "vc_x64_dll" with your specific build directory 
#set(wxWidgets_LIB_DIR "${wxWidgets_ROOT_DIR}/lib/win32") 
#
#include_directories("${wxWidgets_ROOT_DIR}/include" "${wxWidgets_LIB_DIR}/mswud")
#link_directories("${wxWidgets_LIB_DIR}")
#
## You must manually type the names of the core .lib files you need
#target_link_libraries(your_target_name 
#    wxmsw32u_core.lib 
#    wxbase32u.lib
#)

# extra lib
#svs_add_to_target_lib(wxmsw32ud_core.lib wxbase32ud.lib wxbase32ud_net.lib wxbase32ud_xml.lib wxmsw32ud_adv.lib wxmsw32ud_aui.lib wxmsw32ud_gl.lib wxmsw32ud_html.lib wxmsw32ud_media.lib wxmsw32ud_propgrid.lib wxmsw32ud_ribbon.lib wxmsw32ud_richtext.lib wxmsw32ud_stc.lib wxmsw32ud_webview.lib wxmsw32ud_xrc.lib)

## 1. TELL CMAKE WHERE WXWIDGETS IS (Change these paths to your actual paths)
#set(wxWidgets_ROOT_DIR "D:/Work/Projects/Imports_v2/wxWidgets/3.2.11/vs2015")
## Explicitly request shared (DLL) libraries
#set(wxWidgets_USE_SHARED ON)
## Explicitly ensure matching MSVC runtime linkage (Dynamic/MD for DLLs)
#set(wxWidgets_USE_STATIC_RUNTIME OFF)
## Enable C++14 flags for MSVC 2015
#if(MSVC)
#    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14")
#endif()
## Find wxWidgets components
#find_package(wxWidgets REQUIRED core base)
#include(${wxWidgets_USE_FILE})
#set(wxWidgets_CONFIGURATION mswud)
#
## If the automated script still fails, you can skip find_package entirely 
## and map the "32" files manually:
#if(NOT wxWidgets_FOUND)
#    set(wxWidgets_INCLUDE_DIRS 
#        "${wxWidgets_ROOT_DIR}/include"
#        "${wxWidgets_ROOT_DIR}/lib/win32/mswu"
#    )
#    set(wxWidgets_LIBRARIES
#        "${wxWidgets_ROOT_DIR}/lib/x64/wxbase32u.lib"
#        "${wxWidgets_ROOT_DIR}/lib/x64/wxmsw32u_core.lib"
#    )
#    set(wxWidgets_FOUND TRUE)
#endif()







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

# Add this line to force the Visual Studio linker to search your folder
target_link_directories(History PRIVATE "D:/Work/Projects/Imports_v2/wxWidgets/3.2.11/lib/vc14x_dll")

#set(WX_LIBS
#	"${wxWidgets_LIB_DIR}/wxbase32ud.lib"
#	"${wxWidgets_LIB_DIR}/wxbase32ud_net.lib"
#	"${wxWidgets_LIB_DIR}/wxbase32ud_xml.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_core.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_adv.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_aui.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_gl.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_html.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_media.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_propgrid.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_ribbon.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_richtext.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_stc.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_webview.lib"
#	"${wxWidgets_LIB_DIR}/wxmsw32ud_xrc.lib"
#)
#
#target_link_libraries(History PRIVATE ${WX_LIBS}) 

	
# Assets ----------------------------------------------------------------------
svs_add_asset("Resources/Icon.ico" "." RUNTIME)