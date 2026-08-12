# Usage: 
# FILE = zip to unzip, without extension
# SVS_CMAKE_HELPER_DIR = path to platform unzip scripts
get_filename_component(zipPath ${FILE} DIRECTORY)
get_filename_component(zipWE ${FILE} NAME)
get_filename_component(FILE "${FILE}.zip" ABSOLUTE)

# Function to do the unzipping
function(unzip_and_touch)
   message(STATUS "Extracting ${FILE}...")
	if(WIN32)
		set(cmd powershell -executionpolicy bypass -file ${SVS_CMAKE_HELPER_DIR}/unzip.ps1)
	else()
		set(cmd bash ${SVS_CMAKE_HELPER_DIR}/unzip.sh)
   endif()

   execute_process(
      COMMAND ${cmd} ${zipPath}/${zipWE}
      WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
   )
   execute_process(
      COMMAND ${CMAKE_COMMAND} -E touch ${zipPath}/${zipWE}
      WORKING_DIRECTORY ${zipPath}
   )
endfunction()

# First check if the zip file is present
if(EXISTS ${FILE} AND NOT EXISTS ${zipPath}/${zipWE}) # Need to unzip
   unzip_and_touch()
elseif(EXISTS ${zipPath}/${zipWE} AND EXISTS ${FILE}) # Already unzipped - check timestamps
   if(${FILE} IS_NEWER_THAN ${zipPath}/${zipWE})
      unzip_and_touch()
   endif()
elseif(NOT EXISTS ${zipPath}/${zipWE}) # Fail (should never happen - this is checked before invocation in svs_imports)
   message(FATAL_ERROR "The library does not exist in imports: " ${zipPath}/${zipWE} " or ${FILE}")
endif()