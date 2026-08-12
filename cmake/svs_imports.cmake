# -----------------------------------------------------------------------------
function(_svs_find_lib linking_type text_name lib_name the_dir)
	set(vname IMPORT_${text_name}_${lib_name}_LIB)

	if(DEFINED ${vname}) 
		return()
	endif()

	set(debug_dir ${the_dir}/lib/${SVS_IMPORTS_ARCH}/debug)
	set(release_dir ${the_dir}/lib/${SVS_IMPORTS_ARCH}/release)
	set(generic_dir ${the_dir}/lib/${SVS_IMPORTS_ARCH})

	if(UNIX)
		if("${linking_type}" STREQUAL "STATIC")
			set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
		else()
			set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")
		endif()
	else()
		if("${linking_type}" STREQUAL "DYNAMIC")
			if("${lib_name}" MATCHES "-static")
				message(FATAL_ERROR "Specifying static library with dynamic linking! - ${lib_name}")
			endif()
		endif()
		if("${linking_type}" STREQUAL "STATIC")
			if(NOT("${lib_name}" MATCHES "-static"))
				message("\nLibrary linked statically does not meet the standard naming (missing '-static' suffix) - ${lib_name}\n")
			endif()
		endif()
	endif()

	find_library(${vname}_lib ${lib_name} ${generic_dir} NO_DEFAULT_PATH)
	if(${vname}_lib)
		message(STATUS "* " ${text_name} " (" ${lib_name} ") generic was found")
		set(${vname}   ${${vname}_lib} CACHE STRING "")
		set(${vname}_D ${${vname}_lib} CACHE STRING "")

		message(STATUS "  " ${${vname}})

	else()
		find_library(${vname}_lib_r ${lib_name} ${release_dir} NO_DEFAULT_PATH)
		find_library(${vname}_lib_d ${lib_name} ${debug_dir} NO_DEFAULT_PATH)

		if(NOT ${vname}_lib_r OR NOT ${vname}_lib_d)
			file(REMOVE_RECURSE ${the_dir}/lib/${SVS_IMPORTS_ARCH})
			message(FATAL_ERROR ${text_name} " was NOT found "
				"\n-generic: " ${generic_dir}/${lib_name}
				"\n-release: " ${release_dir}/${lib_name}
				"\n-debug:   " ${debug_dir}/${lib_name}
				"\n Cleaning library directory in case of bad build..."
			)
		endif()

		message(STATUS "* " ${text_name} " (" ${lib_name} ") debug and release were found")
		set(${vname}   ${${vname}_lib_r} CACHE STRING "")
		set(${vname}_D ${${vname}_lib_d} CACHE STRING "")

		message(STATUS "  " ${${vname}_D})
		message(STATUS "  " ${${vname}})
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(_add_to_copy the_dir)
	# Using a global, inter-project variable
	get_property(LIBS_TO_COPY GLOBAL PROPERTY GLOBAL_LIBS_TO_COPY)
	set_property(GLOBAL PROPERTY GLOBAL_LIBS_TO_COPY ${LIBS_TO_COPY} ${the_dir})
endfunction()

# -----------------------------------------------------------------------------
function(_add_unzip_command lib_name the_dir)
	if(NOT EXISTS ${the_dir}.zip AND NOT EXISTS ${the_dir})
		message(FATAL_ERROR "The library does not exist in imports: " ${the_dir}.zip)
   endif()
   get_filename_component(zipFile "${the_dir}" ABSOLUTE)
   set(stampFile "${CMAKE_BINARY_DIR}/${lib_name}.stamp")
   add_custom_command(OUTPUT "${stampFile}"
      COMMAND ${CMAKE_COMMAND} ARGS
            -DFILE=${zipFile}
            -DSVS_CMAKE_HELPER_DIR=${SVS_CMAKE_HELPER_DIR}
            -DSTAMP=${stampFile}
            -P "${SVS_CMAKE_HELPER_DIR}/svs_unzip.cmake"
   )
   execute_process(COMMAND ${CMAKE_COMMAND}
         -DFILE=${zipFile}
         -DSVS_CMAKE_HELPER_DIR=${SVS_CMAKE_HELPER_DIR}
         -DSTAMP=${stampFile}
         -P "${SVS_CMAKE_HELPER_DIR}/svs_unzip.cmake"
         WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
   )
endfunction()

# -----------------------------------------------------------------------------
function(_svs_build_lib lib_name lib_ver)
	get_filename_component(base_dir ${IMPORTS_DIR}/${lib_name}/ ABSOLUTE)
	message(STATUS ${SVS_IMPORTS_COMPILER_FULL})
	if(WIN32)
		set(cmd powershell -executionpolicy bypass -file ${base_dir}/build_command.ps1)
	else()
		set(cmd bash ${base_dir}/build_command.sh)
	endif()
	message(STATUS "Executing build command: ${cmd} ${base_dir} ${lib_name} ${lib_ver} ${SVS_IMPORTS_COMPILER_FULL} ${SVS_IMPORTS_ARCH} ${CMAKE_TOOLCHAIN_FILE}")
	execute_process(COMMAND ${cmd} ${base_dir} ${lib_name} ${lib_ver} ${SVS_IMPORTS_COMPILER_FULL} ${SVS_IMPORTS_ARCH} ${CMAKE_TOOLCHAIN_FILE}
					WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
				)
endfunction()

# -----------------------------------------------------------------------------
function(_svs_check_build_lib lib_name lib_ver)
	set(base_dir ${IMPORTS_DIR}/${lib_name}/)
	set(prefix ${IMPORTS_DIR}/${lib_name}/${lib_ver}/)
	set(arch_dir ${prefix}/${SVS_IMPORTS_COMPILER_FULL}/lib/${SVS_IMPORTS_ARCH})
	if(NOT EXISTS ${base_dir})
		message(FATAL_ERROR "Library does not exist: " ${base_dir})
	endif()
	if(NOT WIN32)
		if(EXISTS ${prefix}/${SVS_IMPORTS_COMPILER_FULL} OR
				EXISTS ${prefix}/${SVS_IMPORTS_COMPILER_FULL}.zip OR
				EXISTS ${prefix}/all.zip)
			#DO NOTHING - PRE-COMPILED LIBRARY FOUND (ZIPPED OR IN FOLDER)
			message(STATUS "Found ${lib_name}, not building")
		elseif(EXISTS ${base_dir}/build_command.sh OR EXISTS ${base_dir}/build_command.ps1)	
			message(STATUS "No pre-compiled version of " ${lib_name} " found, attempting to build from source...")
			_svs_build_lib(${lib_name} ${lib_ver})
		else()
			message(FATAL_ERROR "No suitable version of " ${lib_name} " found in prefix: " ${prefix} ", and no build scripts found.")
		endif()
	else()
		if(EXISTS ${arch_dir} OR
				EXISTS ${prefix}/${SVS_IMPORTS_COMPILER_FULL}.zip OR
				EXISTS ${prefix}/all.zip OR
				EXISTS ${prefix}/win.zip) 
			message(STATUS "Found ${lib_name}, not building")
		elseif(EXISTS ${base_dir}/build_command.sh OR EXISTS ${base_dir}/build_command.ps1)	
			message(STATUS "No pre-compiled version of " ${lib_name} " found, attempting to build from source...")
			_svs_build_lib(${lib_name} ${lib_ver})
		else()
			message(FATAL_ERROR "No suitable version of " ${lib_name} " found in prefix: " ${prefix} ", and no build scripts found.")
		endif()
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(_svs_locate_lib_dir lib_name lib_ver)
	set(prefix ${IMPORTS_DIR}/${lib_name}/${lib_ver}/)
	if(NOT EXISTS ${prefix})
		message(FATAL_ERROR "Given version of library does not exist: " ${prefix})
	endif()

	if(EXISTS ${prefix}/${SVS_IMPORTS_COMPILER_FULL}.zip OR EXISTS ${prefix}/${SVS_IMPORTS_COMPILER_FULL})
		get_filename_component(the_dir ${prefix}/${SVS_IMPORTS_COMPILER_FULL} ABSOLUTE)
	elseif(WIN32 AND (EXISTS ${prefix}/win.zip))
		get_filename_component(the_dir ${prefix}/win ABSOLUTE)
	elseif(EXISTS ${prefix}/all.zip)
		get_filename_component(the_dir ${prefix}/all ABSOLUTE)
	else()
		message(FATAL_ERROR "No suitable version of " ${lib_name} " found in: " ${prefix})
	endif()

	_add_unzip_command(${lib_name} ${the_dir})

   set(IMPORTS_DEPENDENCIES ${IMPORTS_DEPENDENCIES} "${CMAKE_BINARY_DIR}/${lib_name}.stamp" PARENT_SCOPE)
	set(IMPORT_${lib_name}_DIRECTORY ${the_dir} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_import_test linking name version)
	# Dirty hack to avoid linking test libs to real versions
	set(bkpLibs ${ALL_${PROJECT_NAME}_LIBS})
	set(ALL_${PROJECT_NAME}_LIBS "")
	svs_import(${linking} ${name} ${version} ${ARGN})
	set(TEST_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS} PARENT_SCOPE)
	set(ALL_${PROJECT_NAME}_LIBS ${bkpLibs} PARENT_SCOPE)

endfunction()

# -----------------------------------------------------------------------------
function(svs_import linking name version)
	if(NOT DEFINED IMPORTS_DIR)
		message(FATAL_ERROR "IMPORTS_DIR is not defined")
	endif()

	if(NOT ("${linking}" STREQUAL "STATIC"  OR
			"${linking}" STREQUAL "DYNAMIC" OR
			"${linking}" STREQUAL "SYSTEM"  OR
			"${linking}" STREQUAL "HEADERS"))
		message(FATAL_ERROR "Unknown liniking type " ${linking} 
						",\n available options: STATIC, DYNAMIC, SYSTEM, HEADERS")
	endif()


	# Store the exact library version used
	if(USED_IMPORTS_${name} AND NOT("${USED_IMPORTS_${name}}" STREQUAL "${version}"))
		message(FATAL_ERROR "${name} already included in different version: ${USED_IMPORTS_${name}}, requested: ${version}")
	endif()
	set(USED_IMPORTS ${REQUIRE_IMPORTS} ${name} PARENT_SCOPE)
	set(USED_IMPORTS_${name} ${version} PARENT_SCOPE)

	if("${linking}" STREQUAL "SYSTEM")
		_svs_import_system(${name} ${linking} ${ARGN})
		return()
	endif()

	# Try to import with custom lib settings
	start_track_variables() # That is awful but necessary
	_svs_import_custom(${linking} ${name} ${version} ${ARGN})
	forward_changed_variables_to_parent_scope()

	if("${SVS_IMPORTED_CUSTOM_${name}}" STREQUAL "yes")
		return()
	endif()

	# Check if libraries should be built
	if(NOT "${linking}" STREQUAL "HEADERS")
		_svs_check_build_lib(${name} ${version})
	endif()

	# Check if HEADERS means only headers and vice versa
	if((NOT "${linking}" STREQUAL "HEADERS") AND (ARGC EQUAL 3))
		message(FATAL_ERROR "No files to link in " ${name})
	endif()

	if(("${linking}" STREQUAL "HEADERS") AND (ARGC GREATER 3))
		message(FATAL_ERROR "Linking files when headers only expected in " ${name})
	endif()

	_svs_locate_lib_dir(${name} ${version})
	set(IMPORTS_DEPENDENCIES ${IMPORTS_DEPENDENCIES} PARENT_SCOPE)
	
	if (EXISTS ${IMPORT_${name}_DIRECTORY}/include)
		if ((WIN32) AND (EXISTS ${IMPORT_${name}_DIRECTORY}/include/${SVS_IMPORTS_ARCH}))
			include_directories(${IMPORT_${name}_DIRECTORY}/include/${SVS_IMPORTS_ARCH})
		else()
			include_directories(${IMPORT_${name}_DIRECTORY}/include)
		endif()
	endif()

	# If libraries names were passed, link them
	if(NOT "${linking}" STREQUAL "HEADERS")
	
		if("${linking}" STREQUAL "DYNAMIC")
			if(WIN32)
				# Currently only copies release DLLs - copy script will check the path format and attempt to copy debug also
				if(EXISTS ${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH}/release)
					_add_to_copy(${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH}/release)
				elseif(EXISTS ${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH})
					_add_to_copy(${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH})
				else()
					_add_to_copy(${IMPORT_${name}_DIRECTORY}/bin)
				endif()
			else()
				_add_to_copy(${IMPORT_${name}_DIRECTORY}/lib)
			endif()
		endif()

		foreach(f ${ARGN})
			_svs_find_lib(${linking} ${name} ${f} ${IMPORT_${name}_DIRECTORY})
			set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS}
					debug     ${IMPORT_${name}_${f}_LIB_D}
					optimized ${IMPORT_${name}_${f}_LIB}
				)
		endforeach()
		set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS} PARENT_SCOPE)
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(_svs_import_custom linking name version)
	set(SVS_IMPORTED_CUSTOM_${name} no PARENT_SCOPE)

	if("${name}" STREQUAL "boost")
		# Check if libraries should be built
		_svs_check_build_lib(${name} ${version})
		if(ARGC EQUAL 0)
			message(FATAL_ERROR "Boost required but no component specified")
		endif()

		set(BOOST_ROOT ${IMPORTS_DIR}/${name}/${version}/${SVS_IMPORTS_COMPILER_FULL})

		message(STATUS "BOOST ROOT: " ${BOOST_ROOT})

		_add_unzip_command("boost" ${BOOST_ROOT})
		set(IMPORTS_DEPENDENCIES ${IMPORTS_DEPENDENCIES} "${CMAKE_BINARY_DIR}/boost.stamp" PARENT_SCOPE)
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib/${SVS_IMPORTS_ARCH})

		if("${linking}" STREQUAL "STATIC")
			set(Boost_USE_STATIC_LIBS ON)
		endif()

		link_directories(${BOOST_LIBRARYDIR})

		if(WIN32)
			if(NOT Boost_INCLUDE_DIR)
				set(Boost_NO_SYSTEM_PATHS ON)
				find_package(Boost COMPONENTS ${ARGN} REQUIRED)
			endif()
		else()
			set(Boost_NO_SYSTEM_PATHS ON)
			find_package(Boost COMPONENTS ${ARGN} REQUIRED)
		endif()

		if(NOT Boost_INCLUDE_DIR)
			return() # Do not cache the result
		endif()

		set(Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR} CACHE INTERNAL "")
		set(Boost_LIBRARIES ${Boost_LIBRARIES} CACHE INTERNAL "")

		if("${linking}" STREQUAL "DYNAMIC")
			add_definitions(-DBOOST_ALL_DYN_LINK)
			_add_to_copy(${BOOST_LIBRARYDIR})
		endif()

		include_directories(${Boost_INCLUDE_DIR})

		set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS}
				${Boost_LIBRARIES}
				PARENT_SCOPE
			)

		# Mark the library imported
		set(SVS_IMPORTED_CUSTOM_${name} yes PARENT_SCOPE)
		return()
	endif()

	if("${name}" STREQUAL "Qt5")
		if(NOT "${linking}" STREQUAL "DYNAMIC")
			message(FATAL_ERROR "Qt5 has to be linked dynamically")
		endif()

		# Check if libraries should be built
		_svs_check_build_lib("Qt" ${version})

		set(QT_ROOT ${IMPORTS_DIR}/Qt/${version}/${SVS_IMPORTS_COMPILER_FULL})

		_add_unzip_command("Qt" ${QT_ROOT})
		set(IMPORTS_DEPENDENCIES ${IMPORTS_DEPENDENCIES} "${CMAKE_BINARY_DIR}/Qt.stamp" PARENT_SCOPE)

		set(CMAKE_PREFIX_PATH ${QT_ROOT}/${SVS_IMPORTS_ARCH})
		if(WIN32)
			_add_to_copy(${QT_ROOT}/${SVS_IMPORTS_ARCH}/bin)
		else()
			_add_to_copy(${QT_ROOT}/${SVS_IMPORTS_ARCH}/lib)
		endif()

		if(ARGC EQUAL 0)
			message(FATAL_ERROR "Qt required but no component specified")
		endif()

		add_definitions(-DQT_DLL)

		start_track_variables()
		find_package(Qt5 COMPONENTS ${ARGN} REQUIRED)
		forward_changed_variables_to_parent_scope()

		if(NOT SVS_QT_FOUND)
			set(SVS_QT_FOUND true CACHE INTERNAL "")

			message(STATUS "* Added QT5 libraries")
			foreach(l ${ARGN})
				message(STATUS "*    " ${l})
			endforeach()
		endif()

		# Mark the library imported
		set(SVS_IMPORTED_CUSTOM_${name} yes PARENT_SCOPE)
		return()
	endif()
	
	if("${name}" STREQUAL "Leadtools")
		if("${linking}" STREQUAL "DYNAMIC")
			message(FATAL_ERROR "You must use Static linking for Leadtools. It has DLLs, but they are registered with Windows and should not be part of the CopyDependencies step.")
		endif()
		# Check if libraries should be built
		_svs_check_build_lib(${name} ${version})
	endif()

	if("${name}" STREQUAL "wxWidgets")
		# Check if libraries should be built
		_svs_check_build_lib(${name} ${version})
		if(UNIX)
			if("${SVS_TARGET_ARCH}" STREQUAL "armv7")
				message(FATAL_ERROR "WxWidgets not yet supported under ARM")
			endif()
			
			if("${linking}" STREQUAL "STATIC")
				message(FATAL_ERROR "wxWidgets must be linked dynamically")
			endif()

			# Define a couple of flags and use the default code
			add_definitions(
					-D_FILE_OFFSET_BITS=64
					-DWXUSINGDLL
					-D__WXGTK__
				)
		else()
			_svs_locate_lib_dir(${name} ${version})
			include_directories(${IMPORT_${name}_DIRECTORY}/include)
			include_directories(${IMPORT_${name}_DIRECTORY}/lib/${SVS_IMPORTS_ARCH}/mswu)
			link_directories(${IMPORT_${name}_DIRECTORY}/lib/${SVS_IMPORTS_ARCH}/)

			if(ARGC GREATER 3)
				message(FATAL_ERROR "wxWidgets specifies libraries to link with #pragma")
			endif()

			# Mark the library imported
			set(SVS_IMPORTED_CUSTOM_${name} yes PARENT_SCOPE)
		endif()
		return()
	endif()

	if("${name}" STREQUAL "CrashRpt")
		# Check if libraries should be built
		_svs_check_build_lib(${name} ${version})
		if(NOT WIN32)
			message(FATAL_ERROR "CrashRpt is a Windows-only library")
		endif()

		if(NOT "${linking}" STREQUAL "DYNAMIC")
			message(FATAL_ERROR "CrashRpt must be linked dynamically")
		endif()

		_svs_locate_lib_dir(${name} ${version})

		link_directories(${IMPORT_${name}_DIRECTORY}/lib/${SVS_IMPORTS_ARCH}/)
		svs_add_asset("${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH}/CrashSender1402.exe" "." RUNTIME)
		svs_add_asset("${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH}/crashrpt_lang.ini" "." RUNTIME)

		# We don't actually want to mark the library as imported here. Allow the default stuff to happen.
		#set(SVS_IMPORTED_CUSTOM_${name} yes PARENT_SCOPE)
		return()
	endif()
	
	if("${name}" STREQUAL "eBUS_SDK")
		_svs_locate_lib_dir(${name} ${version})
		
		if(UNIX)		
			set(target_dir "../lib/")			
			set(source_dir "${IMPORT_${name}_DIRECTORY}/lib/genicam/xml")
			svs_add_asset("${source_dir}" "${target_dir}" RUNTIME)
			
			set(source_dir "${IMPORT_${name}_DIRECTORY}/config")
			svs_add_asset("${source_dir}/Viento-G-640-Plc002-Superframe_Data16.pvcfg" "${target_dir}" RUNTIME)
		else()
			if("${SVS_IMPORTS_ARCH}" STREQUAL "win32")
				set(target_dir "GenICam/bin/Win32_i86")
			elseif("${SVS_IMPORTS_ARCH}" STREQUAL "x64")
				set(target_dir "GenICam/bin/Win64_x64")
			endif()
			
			set(source_dir "${IMPORT_${name}_DIRECTORY}/bin/${SVS_IMPORTS_ARCH}/GenICam/")
			
			svs_add_asset("${source_dir}/CLProtocol_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			svs_add_asset("${source_dir}/GCBase_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			svs_add_asset("${source_dir}/GenApi_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			svs_add_asset("${source_dir}/Log_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			svs_add_asset("${source_dir}/MathParser_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			svs_add_asset("${source_dir}/NodeMapData_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			svs_add_asset("${source_dir}/XmlParser_MD_VC120_v3_0_PT.dll" "." RUNTIME)
			
			set(source_dir "${IMPORT_${name}_DIRECTORY}/config/")
			svs_add_asset("${source_dir}/Viento-G-640-Plc002-Superframe_Data16.pvcfg" "." RUNTIME)
		endif()
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(_svs_import_system name linking)

	if(WIN32)
		message(FATAL_ERROR "svs_import_system not allowed - windows doesn't use shared system libraries")
		# Well, it could work but we don't want to make it more confusing than it already is
	endif()

	if("${name}" STREQUAL "boost")
		if(ARGC EQUAL 0)
			message(FATAL_ERROR "Boost required but no component specified")
		endif()

		set(BOOST_ROOT ${IMPORTS_DIR}/${name}/${version}/${SVS_IMPORTS_COMPILER_FULL})
		set(BOOST_LIBRARYDIR ${BOOST_ROOT}/lib/${SVS_IMPORTS_ARCH})

		
		link_directories(${BOOST_LIBRARYDIR})

		if(NOT Boost_INCLUDE_DIR)
			find_package(Boost COMPONENTS ${ARGN} REQUIRED)
		endif()

		if(NOT Boost_INCLUDE_DIR)
			return() # Do not cache the result
		endif()

		set(Boost_INCLUDE_DIR ${Boost_INCLUDE_DIR} CACHE INTERNAL "")
		set(Boost_LIBRARIES ${Boost_LIBRARIES} CACHE INTERNAL "")

		if("${linking}" STREQUAL "DYNAMIC")
			add_definitions(-DBOOST_ALL_DYN_LINK)
		endif()

		include_directories(${Boost_INCLUDE_DIR})

		set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS}
				${Boost_LIBRARIES}
				PARENT_SCOPE
			)

		return()
	endif()

	if("${name}" STREQUAL "Qt5")

		if(ARGC EQUAL 0)
			message(FATAL_ERROR "Qt required but no component specified")
		endif()

		add_definitions(-DQT_DLL)

		start_track_variables()
		find_package(Qt5 COMPONENTS ${ARGN} REQUIRED)
		forward_changed_variables_to_parent_scope()

		if(NOT SVS_QT_FOUND)
			set(SVS_QT_FOUND true CACHE INTERNAL "")

			message(STATUS "* Added QT5 libraries")
			foreach(l ${ARGN})
				message(STATUS "*    " ${l})
			endforeach()
		endif()

		return()
	endif()

	if("${name}" STREQUAL "glew")
		find_package(GLEW)
		message(STATUS "* glew generic was found\n  " ${GLEW_LIBRARIES})
		set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS}
				debug     ${GLEW_LIBRARIES}
				optimized ${GLEW_LIBRARIES}
				PARENT_SCOPE
			)
		return()
	endif()

	set(CMAKE_FIND_LIBRARY_SUFFIXES ".so")

	# If libraries names were passed, link them
	if(ARGC GREATER 1)
		foreach(f ${ARGN})
			find_library(${name}_lib ${f})
			if(${name}_lib)
				set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS}
						debug     ${${name}_lib}
						optimized ${${name}_lib}
					)
				message(STATUS "* " ${name} " (" ${f} ") was found")
				message(STATUS "  " ${${name}_lib})
			else()
				message(FATAL_ERROR ${name} " (" ${f} ") was NOT found")
			endif()
		endforeach()
		set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS} PARENT_SCOPE)
	else()
		message(FATAL_ERROR "No libraries specified for ${name}...")
	endif()

endfunction()

# -----------------------------------------------------------------------------

function(svs_require_imports name action)
	if(NOT( ("${action}" STREQUAL "ANY" AND ARGC EQUAL 2) OR
			("${action}" STREQUAL "BETWEEN" AND ARGC EQUAL 4) OR
			("${action}" STREQUAL "EXACT" AND ARGC EQUAL 3)))
		message(FATAL_ERROR "Unknown action ${action} or wrong no. of parameters, available actions: ANY, BETWEEN, EXACT")
	endif()

	set(REQUIRE_IMPORTS ${REQUIRE_IMPORTS} ${name} PARENT_SCOPE)
	set(REQUIRE_IMPORTS_${name} ${ARGN} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(_validate_imports)
	foreach(l ${REQUIRE_IMPORTS})

		if(NOT (USED_IMPORTS_${l}))
			message(FATAL_ERROR "${l} library is not imported")
		endif()

		list(LENGTH REQUIRE_IMPORTS_${l} len)
		if(len EQUAL 1)
			if(NOT(${REQUIRE_IMPORTS_${l}} STREQUAL ${USED_IMPORTS_${l}}))
				message(FATAL_ERROR "${l} required in different version (${REQUIRE_IMPORTS_${l}} and ${USED_IMPORTS_${l}})")
			endif()
		elseif(len EQUAL 2)
			list(GET REQUIRE_IMPORTS_${l} 0 min)
			list(GET REQUIRE_IMPORTS_${l} 1 max)

			if(min VERSION_GREATER ${USED_IMPORTS_${l}})
				message(FATAL_ERROR "${l} version too low, required: <= ${min}, used: ${USED_IMPORTS_${l}}")
			endif()
			if(max VERSION_LESS ${USED_IMPORTS_${l}})
				message(FATAL_ERROR "${l} version too high, required: >= ${max}, used: ${USED_IMPORTS_${l}}")
			endif()
		endif()
	endforeach()

endfunction()

# -----------------------------------------------------------------------------

