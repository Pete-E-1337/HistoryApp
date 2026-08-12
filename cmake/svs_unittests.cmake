function(svs_addtests testMain appMain)
   if(ARGC GREATER 2)
      set(perfTests ${ARGV2})
   endif()

	# Include version cpp
	if(NOT "${${PROJECT_NAME}_VERSION_DONE}")
		message("\nWarning: No product version specified: ${PROJECT_NAME}\n")
	else()
		set(ALL_${PROJECT_NAME}_SOURCES
			${ALL_${PROJECT_NAME}_SOURCES}
			"${CMAKE_BINARY_DIR}/${PROJECT_NAME}_version.cpp")
		include_directories(${CMAKE_BINARY_DIR})
	endif()

	# Process file names
	get_filename_component(testMain ${testMain} ABSOLUTE)
	get_filename_component(appMain ${appMain} ABSOLUTE)

	# Check each source file in current build
	foreach(src ${ALL_${PROJECT_NAME}_SOURCES})
		get_filename_component(srcFile ${src} NAME_WE)
		get_filename_component(srcDir ${src} DIRECTORY)
		get_filename_component(srcExt ${src} EXT)
		# Don't include the main file
		if(NOT src STREQUAL appMain)
			list(APPEND testSrc ${src})
		endif()
		# Check for a corresponding test file
		# if (NOT srcExt STREQUAL "h" AND NOT srcExt STREQUAL "hpp")
		if(EXISTS ${srcDir}/${srcFile}-test.cpp)
			message(STATUS "Test file found for " ${src})
			list(APPEND testSrc ${srcDir}/${srcFile}-test.cpp)
			list(APPEND testSrcGrp ${srcDir}/${srcFile}-test.cpp)
		else()
			#message(STATUS "No tests specified for " ${src})
		endif()
		# endif()
	endforeach()

	# Add all test cases from static libs
	file(GLOB static_lib_test_list "${CMAKE_BINARY_DIR}/*_testcases.txt")
	foreach(file ${static_lib_test_list})
		file(READ ${file} test_cases)
      list(APPEND testSrc ${test_cases})
      list(APPEND testSrcGrp ${test_cases})
	endforeach()

	# Add the main testing file
	if (EXISTS ${testMain})
		list(APPEND testSrc ${testMain})
		list(APPEND testSrcGrp ${testMain})
	else()
		message(FATAL_ERROR "Could not find main test file: " ${testMain})
	endif()

	# Display properly in IDEs
   source_group("Test Files" FILES ${testSrcGrp})
   
   # Trigger performance test flags
   if (perfTests)
      set_property(SOURCE ${testSrcGrp} PROPERTY COMPILE_DEFINITIONS RUN_PERFORMANCE_TESTS)
   endif()

	# Add build exe
	add_executable(${PROJECT_NAME}-test EXCLUDE_FROM_ALL ${testSrc})

	# Easy name for invokation in make
	if (NOT WIN32)
		add_custom_target(tests)
		add_dependencies(tests ${PROJECT_NAME}-test)
	endif()

	# Link! Merge in all libs and test libs
	list(APPEND TEST_${PROJECT_NAME}_LIBS ${ALL_${PROJECT_NAME}_LIBS})
	if(NOT "${TEST_${PROJECT_NAME}_LIBS}" STREQUAL "")
		target_link_libraries(${PROJECT_NAME}-test ${TEST_${PROJECT_NAME}_LIBS})
	endif()

endfunction()

# This function just exports a list of test files for building in the main project
function(svs_addtests_static_lib)
	# Check each source file
	foreach(src ${ALL_${PROJECT_NAME}_SOURCES})
		get_filename_component(srcFile ${src} NAME_WE)
		get_filename_component(srcDir ${src} DIRECTORY)
		get_filename_component(srcExt ${src} EXT)
		# Check for a corresponding test file
		if(EXISTS ${srcDir}/${srcFile}-test.cpp)
			message(STATUS "Test file found for " ${src})
			list(APPEND testSrc ${srcDir}/${srcFile}-test.cpp)
		else()
			#message(STATUS "No tests specified for " ${src})
		endif()
	endforeach()

	# Dump to file
	file(WRITE "${CMAKE_BINARY_DIR}/${PROJECT_NAME}_testcases.txt" "")
	foreach(src ${testSrc})
		file(APPEND "${CMAKE_BINARY_DIR}/${PROJECT_NAME}_testcases.txt" "${src};")
	endforeach()	
	message(STATUS "Writing ${testSrc} to ${CMAKE_BINARY_DIR}/${PROJECT_NAME}_testcases.txt")

endfunction()