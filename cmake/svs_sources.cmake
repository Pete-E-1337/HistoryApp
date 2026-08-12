# -----------------------------------------------------------------------------
function(svs_src group)
	foreach(f ${ARGN})
		get_filename_component(f ${f} ABSOLUTE)
		set(found false)

		if(EXISTS ${f})
			list(APPEND files ${f})
			set(found true)
		else()
			if(EXISTS ${f}.h)
				list(APPEND files ${f}.h)
				set(found true)
			endif()
			if(EXISTS ${f}.hpp)
				list(APPEND files ${f}.hpp)
				set(found true)
			endif()
			if(EXISTS ${f}.c)
				list(APPEND files ${f}.c)
				set(found true)
			endif()
			if(EXISTS ${f}.cpp)
				list(APPEND files ${f}.cpp)
				set(found true)
			endif()
			if(EXISTS ${f}.inl)
				list(APPEND files ${f}.inl)
				set(found true)
			endif()
		endif()

		if(NOT ${found})
			message(FATAL_ERROR "File not found: " ${f})
		endif()
	endforeach()

	source_group(${group} FILES ${files})
	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} ${files} PARENT_SCOPE)

endfunction()

# -----------------------------------------------------------------------------
function(svs_lib_src group)
	if(NOT DEFINED SVSLIB_DIR)
		message(FATAL_ERROR "SVSLIB_DIR is not defined")
	endif()

	foreach(f ${ARGN})
		svs_src(${group} ${SVSLIB_DIR}/${f})
	endforeach()

	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_auto_lib_src)
	foreach(f ${ARGN})
		get_filename_component(group ${f} DIRECTORY)
		string(REPLACE "/" "\\" group "${group}")
		svs_src("SVSLibrary\\${group}" "${SVSLIB_DIR}/${f}")
	endforeach()

	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_auto_src)
	foreach(f ${ARGN})
		get_filename_component(group ${f} DIRECTORY)
		string(REPLACE "/" "\\" group "${group}")
		svs_src("${group}" ${f})
	endforeach()

	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_lib_use)
	list(APPEND CMAKE_MODULE_PATH ${SVSLIB_DIR}/cmake) # Extend locally only

	start_track_variables() # That is awful but necessary
	foreach(f ${ARGN})
		include("${f}")
	endforeach()
	forward_changed_variables_to_parent_scope()
	# set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} PARENT_SCOPE)
	# message("&&lib_use ${ARGN}: ${ALL_${PROJECT_NAME}_SOURCES}")
endfunction()

# -----------------------------------------------------------------------------
function(svs_glob_src group glob_function)
	message(STATUS "CMake uses glob functions - expect the awesome (and manually re-run cmake if new files are added to globbed directories)")

	foreach(globArg ${ARGN})
		file(${glob_function} globFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${globArg})
		
		if(${SVS_VERBOSE})
			message(STATUS "	Looking for ${globArg}")
		endif()
		
		foreach(globFileName ${globFiles})
			if(${SVS_VERBOSE})
				message(STATUS "		Found ${globFileName}")
			endif()
			
			svs_src("${group}" ${globFileName})
		endforeach()
	endforeach()

	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} ${files} PARENT_SCOPE)
endfunction()

function(svs_auto_glob_src glob_function)
	message(STATUS "CMake uses glob functions - expect the awesome (and manually re-run cmake if new files are added to globbed directories)")

	foreach(globArg ${ARGN})
		file(${glob_function} globFiles RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" ${globArg})
		
		if(${SVS_VERBOSE})
			message(STATUS "	Looking for ${globArg}")
		endif()
		
		foreach(globFileName ${globFiles})
			if(${SVS_VERBOSE})
				message(STATUS "		Found ${globFileName}")
			endif()
			
			get_filename_component(group ${globFileName} DIRECTORY)
			string(REPLACE "/" "\\" group "${group}")
			svs_src("${group}" ${globFileName})
		endforeach()
	endforeach()

	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} ${files} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(_copy_asset_for_runtime location destination)

	# message("copy_asset |${location}| to |${destination}|")

	if(NOT IS_ABSOLUTE "${destination}")
		if(WIN32)
			_vs_get_bin_dir(absDest)
		else()
			set(absDest "${CMAKE_BINARY_DIR}/bin")
		endif()

		set(destination "${absDest}/${destination}")
	endif()


	if(IS_DIRECTORY ${location})
		get_filename_component(absLoc "${location}" DIRECTORY)
	else()
		get_filename_component(absLoc "${location}" ABSOLUTE)
	endif()

	# Make sure we are not overwritting anything with itself
	if(NOT ("${absDest}" STREQUAL "${absLoc}"))

	endif()

	if(IS_DIRECTORY ${location})
		# It's a directory
		add_custom_command(TARGET COPY_DEPENDENCIES POST_BUILD COMMAND
				${CMAKE_COMMAND} -E copy_directory ${location} "${destination}")
	else()
		# It's a file
		add_custom_command(TARGET COPY_DEPENDENCIES POST_BUILD COMMAND
				${CMAKE_COMMAND} -E make_directory "${destination}")

		add_custom_command(TARGET COPY_DEPENDENCIES POST_BUILD COMMAND
				${CMAKE_COMMAND} -E copy ${location} "${destination}")
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(svs_add_asset raw_location destination target)
   _ensure_install_directory_set()

	# Target can be specified as RUNTIME then it will copy the file before running the app

	if("${destination}" STREQUAL "")
		message(FATAL_ERROR "svs_add_asset: Empty destination not supported anymore")
	endif()

	if(NOT (("${target}" STREQUAL RUNTIME) OR ("${target}" STREQUAL INSTALL)))
		message(FATAL_ERROR "svs_add_asset: Unknown target: ${target}")
	endif()

	# Make location absolute
	get_filename_component(location "${raw_location}" ABSOLUTE)

	_create_copy_dependencies_target()

	if("${location}" MATCHES "\\*\\.[a-zA-Z\\*]+$")
		set(det_type "glob")
		file(GLOB files "${location}")
		set(type FILES)
	elseif("${location}" MATCHES "\\*$")
		set(det_type "recursive")
		file(GLOB_RECURSE files "${location}")
		set(type FILES)
	elseif((IS_DIRECTORY "${location}") OR ((NOT EXISTS "${location}") AND ("${raw_location}" MATCHES "\\/$")))
		set(det_type "directory")
		set(files "${location}")
		#_get_last_path_elem("${location}" dirname)
		#set(destination "${destination}/${dirname}")
		set(type DIRECTORY)
	else()
		if(NOT EXISTS "${location}")
			message("svs_add_asset: Cannot find file matching: ${location} (it may need to be generated)")
		endif()
		set(det_type "file")
		set(files "${location}")
		set(type FILES)
	endif()
	
	# message("Installing: ${det_type} ${location} ${destination}")


	get_filename_component(locationDir "${location}" DIRECTORY)
	foreach(fileName ${files})
		if("${det_type}" STREQUAL "recursive")
			get_filename_component(fileDir "${fileName}" DIRECTORY)
			string(REPLACE "${locationDir}" "" installDir "${fileDir}")
			set(fileDest "${destination}${installDir}")
		else()
			set(fileDest "${destination}")
		endif()

		# If RUNTIME, copy the files right after a successful build
		if("${target}" STREQUAL RUNTIME)
			if("${det_type}" STREQUAL "directory")
				_get_last_path_elem("${location}" dirname)
			endif()

			set(cpFileDest "${fileDest}/${dirname}")

			_copy_asset_for_runtime("${fileName}" "${cpFileDest}")
		endif()

		# If destination is absolute don't change it
		if(WIN32)
			set(fileDest "bin/${fileDest}")
		else()
			if(NOT IS_ABSOLUTE "${destination}")
				set(fileDest "${SVS_INSTALL_DIR}/bin/${fileDest}")
				get_filename_component(fileDest "${fileDest}" ABSOLUTE)
			endif()
		endif()

		# Fix windows paths for NSIS
		if(WIN32)
			if("${CPACK_GENERATOR}" STREQUAL "NSIS" OR "${CPACK_GENERATOR}" STREQUAL "")
				string(REPLACE "/" "\\\\" fileDest "${fileDest}")
			endif()
			
			set(installAssetTarget RelWithDebInfo)
		else()
			set(installAssetTarget Release)
		endif()

		# message("cpack install: ${fileName} ${fileDest}")
		install(${type} "${fileName}"
			DESTINATION "${fileDest}"
			CONFIGURATIONS ${installAssetTarget}
		)
		
	endforeach()

endfunction()
# -----------------------------------------------------------------------------

