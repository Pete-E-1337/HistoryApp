
function(glibc_version output_var)
	if(SVS_GLIBC_VERSION)
		set(${output_var} "${SVS_GLIBC_VERSION}" PARENT_SCOPE)
		return()
	endif()

	set(glibc_check_code "
		#include <stdio.h>
		#include <gnu/libc-version.h>
		int main(int i, char** c)
		{
			printf(\"%s\", gnu_get_libc_version());
			return 0;
		}
	")

	file(WRITE "${CMAKE_BINARY_DIR}/glibc_test.c" "${glibc_check_code}")

	enable_language(C)

	try_run(
		run_result_unused
		compile_result_unused
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/glibc_test.c"
		RUN_OUTPUT_VARIABLE res
	)

	message(STATUS "* glibc version: ${res}")

	set(SVS_GLIBC_VERSION "${res}" CACHE INTERNAL "")
	set(${output_var} "${res}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(gcc_version output_var)
	if(SVS_GCC_VERSION)
		set(${output_var} "${SVS_GCC_VERSION}" PARENT_SCOPE)
		return()
	endif()

	set(gcc_check_code "
			#include <stdio.h>
			int main(int i, char** c)
			{
				printf(\"%d.%d\", __GNUC__, __GNUC_MINOR__);
				return 0;
			}
		")

	file(WRITE "${CMAKE_BINARY_DIR}/gcc_test.c" "${gcc_check_code}")

	enable_language(C)

	try_run(
		run_result_unused
		compile_result_unused
		"${CMAKE_BINARY_DIR}"
		"${CMAKE_BINARY_DIR}/gcc_test.c"
		RUN_OUTPUT_VARIABLE res
	)

	message(STATUS "* gcc version: ${res}")

	set(SVS_GCC_VERSION "${res}" CACHE INTERNAL "")
	string(SUBSTRING ${SVS_GCC_VERSION} 0 1 GCC_MJR_VERSION)
	set(SVS_GCC_MAJOR_VERSION "${GCC_MJR_VERSION}" CACHE INTERNAL "")
	set(${output_var} "${res}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(gcc_version_cross output_var)
	# This is not tested on all versions of GCC!
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	string(REGEX REPLACE "\n$" "" GCC_VERSION "${GCC_VERSION}")
	set(SVS_GCC_VERSION "${GCC_VERSION}" CACHE INTERNAL "")
	string(SUBSTRING ${SVS_GCC_VERSION} 0 1 GCC_MJR_VERSION)
	set(SVS_GCC_MAJOR_VERSION "${GCC_MJR_VERSION}" CACHE INTERNAL "")
	set(${output_var} "${GCC_VERSION}" PARENT_SCOPE)
	message(STATUS "* gcc version: ${GCC_VERSION}")
endfunction()

# -----------------------------------------------------------------------------
function(glibc_version_cross output_var)
	# This is super loopy/experimental. Basically trace back the path taken by the compiler
	# Works on Ubuntu 16.04 and 14.04! Can't guarantee other platforms...
	execute_process(COMMAND ${CMAKE_CXX_COMPILER} -print-file-name=libc.so OUTPUT_VARIABLE LIBC_PATH)
	get_filename_component(LIBC_PATH ${LIBC_PATH} ABSOLUTE)
	get_filename_component(LIBC_DIR ${LIBC_PATH} DIRECTORY)
	string(REGEX REPLACE "\n$" "" LIBC_PATH "${LIBC_PATH}")
	file(READ ${LIBC_PATH} LIBC_OUTPUT)
   string(REGEX MATCH "/[^ ]+libc\\.so\\.[0-9]" MATCHED_OUT ${LIBC_OUTPUT})
   execute_process(COMMAND readlink ${MATCHED_OUT} OUTPUT_VARIABLE LIBC_VER)
	string(REGEX MATCH "[0-9]+\\.[0-9]+" LIBC_VER ${LIBC_VER})
	set(${output_var} "${LIBC_VER}" PARENT_SCOPE)
	message(STATUS "* glibc version: ${LIBC_VER}")
endfunction()