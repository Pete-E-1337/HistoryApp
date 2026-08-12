# Example usage:
#	list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../Imports_v2/cmake/)
#	include(svs_helpers)
#
#	svs_init(../../Imports_v2 ../SVSLibrary)
#	# This also sets cpu architecture and compiler
#
#	# Declare version. This will try to read SVN version/branch too
#	svs_version(4 0 1)
#
#	svs_set_configurations(aa bb cc)
#	svs_restrict_platforms(vs2013 linux)
#
#	# Set platorm default compilation flags
#	svs_set_default_flags()
#
#	svs_src("System files\\"
#			stdafx
#	 	)
#	# You don't have to specify the extension, then all {.c,.cpp,.h,.hpp} will be included
#	#  If extension is specified, only the given file will be included
#
#	svs_lib_src("Api Code\\Private\\" 
#			API/BaseAnalytic.h
#			API/BaseApi.h
#			API/BaseDevPayload.h
#		)
# 
#	# Add include dirs only
#	svs_import(HEADERS gdal 1.9.2)
#
#	# Add "include" directories and link specified libraries
#	#  (Win versions may be under different names)
#	#  list of libraries can be empty - then only include directory will be added
#	svs_import(STATIC boost 1.55.0 system thread chrono filesystem)
#
#	# Import headers and link dynamically from imports dir
#	svs_import(DYNAMIC GeographicLib 1.37 Geographic)
#
#	# Link dynamically to system libraries
#	#  The first param (name) is not important, it just hast to be unique and descriptive
#	svs_import(SYSTEM OpenCV opencv_core opencv_imgproc opencv_ml opencv_calib3d opencv_video)
#
#
#	# Generate binary file
#	svs_executable(SentientDemo)
#
#	# Generate .lib/.a library
#	svs_library(SentientDemo)
#
#	# Genearte .dll/.so library
#	svs_library(SentientDemo SHARED)
#	
#	# Add asset file or directory -
#	# First param is existing file location
#	# Second param is new location respective to binary location. I.e.
#	#  .. will be the build folder
#	#  . will be build/Release or debug or bin - right next to .exe
#	# Specify RUNTIME if the file is necessary to run the binary
#	svs_add_asset("Art" ".." INSTALL)
#	svs_add_asset("Bin/FreeSans.ttf" "." RUNTIME)
#
#  # If you want to modify the default install location you can set the SVS_INSTALL_DIRECTORY
#  # variable.
#  # WARNING: SVS_INSTALL_DIR must be set before any calls to svs_add_asset, as the asset's
#  # final destination is set during that call based on the current value of SVS_INSTALL_DIR.
#
#	# Create an installer
#	svs_create_installer()

# -----------------------------------------------------------------------------
include(TargetArch)
include(ForwardVariables)
include(LinuxVersion)
include(CMakeParseArguments)

include(svs_config)
include(svs_sources)
include(svs_imports)
include(svs_compilation)
include(svs_installer)
include(svs_unittests)

_init_svs_helpers("${CMAKE_CURRENT_LIST_DIR}")

