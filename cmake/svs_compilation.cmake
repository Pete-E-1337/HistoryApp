# -----------------------------------------------------------------------------
function(svs_executable name)
	# Second parameter is used in qt apps to hide console windows and should be set to WIN32

	if(NOT DEFINED name)
		message(FATAL_ERROR "Target name must be defined")
	endif()

	# Make sure we are creating only one executable
	set(EXECUTABLE_NAME "${name}" CACHE STRING "")
	if(NOT("${EXECUTABLE_NAME}" STREQUAL "${name}"))
		message(FATAL_ERROR "svs_executable used twice: ${EXECUTABLE_NAME} and ${name}")
	endif()

	# All the other validations
	if(NOT "${CONFIG_DONE}")
		message("\nWarning: No configurations specified: ${PROJECT_NAME}\n")
	endif()
	if(NOT "${${PROJECT_NAME}_VERSION_DONE}")
		message("\nWarning: No product version specified: ${PROJECT_NAME}\n")
	else()
		set(ALL_${PROJECT_NAME}_SOURCES
			${ALL_${PROJECT_NAME}_SOURCES}
			"${CMAKE_BINARY_DIR}/${PROJECT_NAME}_version.cpp")
		include_directories(${CMAKE_BINARY_DIR})
	endif()
	_validate_imports()

	# Store the name for other helpers
	set(${PROJECT_NAME}_TARGET_NAME ${name} CACHE INTERNAL "")
	set(${PROJECT_NAME}_TARGET_TYPE EXECUTABLE CACHE INTERNAL "")

	if("${ALL_${PROJECT_NAME}_SOURCES}" STREQUAL "")
		message(FATAL_ERROR "No source files defined")
	endif()

	if(ARGV1)
		add_executable(${name} ${ARGV1} ${ALL_${PROJECT_NAME}_SOURCES})
	else()
		add_executable(${name} ${ALL_${PROJECT_NAME}_SOURCES})
	endif()
	
	# Set the default VS project
	if ((WIN32) AND (CMAKE_MAJOR_VERSION GREATER 3) OR ((CMAKE_MAJOR_VERSION EQUAL 3) AND (CMAKE_MINOR_VERSION GREATER 5)))
		set_property(DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${name})
	endif()

	if(NOT "${ALL_${PROJECT_NAME}_LIBS}" STREQUAL "")
		target_link_libraries(${name} ${ALL_${PROJECT_NAME}_LIBS})
	endif()

   # Define update imports target for created target
	add_custom_target(${name}_UPDATE_IMPORTS DEPENDS ${IMPORTS_DEPENDENCIES})
	add_dependencies(${name} ${name}_UPDATE_IMPORTS)
	set_target_properties(${name}_UPDATE_IMPORTS PROPERTIES FOLDER ${PREDEFINED_TARGETS_FOLDER}/Update_Imports)

	_add_copy_libs_target(${name} EXE)
	add_dependencies(COPY_DEPENDENCIES ${name}_UPDATE_IMPORTS)
	add_dependencies(COPY_DEPENDENCIES ${name})

	# Target for when we are cross compiling
	if(CMAKE_CROSSCOMPILING)
		if (SVS_COPY_TARGETS)
			_add_cross_compile_copy_libs_target(${name})
			_add_cross_compile_copy_bin_target(${name})
			add_custom_target(CROSS_COMPILE_COPY_ALL)
			add_dependencies(CROSS_COMPILE_COPY_ALL CROSS_COMPILE_COPY_LIB)
			add_dependencies(CROSS_COMPILE_COPY_ALL CROSS_COMPILE_COPY_BIN)
		else()
			message(STATUS "CROSS_COMPILE_COPY targets not generated - use -DSVS_COPY_TARGETS")
		endif()
	endif()
	
	if(WIN32) # Set working directory in VS
		_vs_set_working_directory()
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(svs_library name)
	if(NOT DEFINED name)
		message(FATAL_ERROR "Target name must be defined")
	endif()
	
	# All the other validations
	if(NOT "${CONFIG_DONE}")
		message("\nWarning: No configurations specified: ${PROJECT_NAME}\n")
	endif()
	if(NOT "${${PROJECT_NAME}_VERSION_DONE}")
		message("\nWarning: No product version specified: ${PROJECT_NAME}\n")
	else()
		set(ALL_${PROJECT_NAME}_SOURCES
			${ALL_${PROJECT_NAME}_SOURCES}
			"${CMAKE_BINARY_DIR}/${PROJECT_NAME}_version.cpp")
		include_directories(${CMAKE_BINARY_DIR})
	endif()
	_validate_imports()

	# Store the name for other helpers
	set(${PROJECT_NAME}_TARGET_NAME ${name} PARENT_SCOPE)
	set(${PROJECT_NAME}_TARGET_TYPE LIBRARY PARENT_SCOPE)

	if(ARGV1)
		add_library(${name} ${ARGV1} ${ALL_${PROJECT_NAME}_SOURCES})
		target_link_libraries(${name} ${ALL_${PROJECT_NAME}_LIBS})
	else()
		add_library(${name} ${ALL_${PROJECT_NAME}_SOURCES})
	endif()

   # Define update imports target for created target
   message(STATUS "ADDING ${name}_UPDATE_IMPORTS DEPENDS ${IMPORTS_DEPENDENCIES}")
	add_custom_target(${name}_UPDATE_IMPORTS DEPENDS ${IMPORTS_DEPENDENCIES})
	add_dependencies(${name} ${name}_UPDATE_IMPORTS)
	set_target_properties(${name}_UPDATE_IMPORTS PROPERTIES FOLDER ${PREDEFINED_TARGETS_FOLDER}/Update_Imports)

endfunction()

# -----------------------------------------------------------------------------
function(svs_add_to_target_src)

	# TODO Why do we need this and why Punchy uses svs_src?

	set(ALL_${PROJECT_NAME}_SOURCES ${ALL_${PROJECT_NAME}_SOURCES} ${ARGN} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_add_to_target_lib)
	set(ALL_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS} ${ARGN} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_prepend_to_target_lib)
	set(ALL_${PROJECT_NAME}_LIBS ${ARGN} ${ALL_${PROJECT_NAME}_LIBS} PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
