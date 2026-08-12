# -----------------------------------------------------------------------------
function(svs_copy_libs_for_dll_target binary_name)
		_add_copy_libs_target(${binary_name} LIB)
endfunction()
# -----------------------------------------------------------------------------
function(_add_copy_libs_target binary_name type)
	#type can be EXE OR LIB
	get_property(LIBS_TO_COPY GLOBAL PROPERTY GLOBAL_LIBS_TO_COPY)

	if(NOT DEFINED LIBS_TO_COPY)
		message("No libs have been defined to copy. Make sure that svs_add_copy_target has been called AFTER all svs_imports.")
	endif()

	_create_copy_dependencies_target()

	if(WIN32)
		_vs_get_bin_dir(libdir)
		if("${type}" STREQUAL "EXE")
			set(binary "${libdir}/${binary_name}.exe")
		else()
			set(binary "${libdir}/${binary_name}.dll")
		endif()
	else()
		set(libdir "${CMAKE_BINARY_DIR}/lib")
		if("${type}" STREQUAL "EXE")
			set(binary "${CMAKE_BINARY_DIR}/bin/${binary_name}")
		else()
			set(binary "${CMAKE_BINARY_DIR}/lib/lib${binary_name}.so")
		endif()
	endif()

	# Get list ready to pass as argument
	get_property(LIB_DIRECTORIES GLOBAL PROPERTY GLOBAL_LIBS_TO_COPY)
	set(libdirs "emptyDir")
	foreach(libdir ${LIB_DIRECTORIES})
		set(libdirs "${libdirs}\\;${libdir}")
	endforeach(libdir)

	if(NOT WIN32)
		# Add additional quoting on Linux
		set(binary "\"${binary}\"")
		set(libdir "\"${libdir}\"")
		# set(libdirs "\"${libdirs}\"")
		set(MSVC_VERSION "\"${MSVC_VERSION}\"")
		# Choose the correct LDD
		if(CMAKE_CROSSCOMPILING)
			set(LDD_CMD ${SVS_LDD})
		else()
			set(LDD_CMD "\"ldd\"")
		endif()
	endif()

	# Add a custom command after building to copy libs
	add_custom_command(TARGET COPY_DEPENDENCIES POST_BUILD
			COMMAND ${CMAKE_COMMAND} ARGS
					-DBIN="${binary}"
					-DLIBDIRS="${libdirs}"
					-DOUTLIBDIR="${libdir}"
					-DMSVC_VERSION="${MSVC_VERSION}"
					-DLDD_CMD="${LDD_CMD}"
					-DIMPORTS_DIR="${IMPORTS_DIR}"
					-P "${SVS_CMAKE_HELPER_DIR}/svs_copylibs.cmake"
		)

	if(UNIX)
		_create_runner_script()
	endif()

endfunction()

# -----------------------------------------------------------------------------
function(svs_build_project name path cmake_args compile_args)
	# e.g. svs_build_project(test "../TestClient" "-DKESTREL_BUILD=yes" "")
	# TODO Check if name contains no spaces

	string(TOUPPER ${name} nameUp)
	set(target_name "COMPILE_${nameUp}")
	set(path "${CMAKE_SOURCE_DIR}/${path}")

	add_custom_target(${target_name} ALL)

	add_custom_command(TARGET ${target_name} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/${name}"
		)
        
	foreach(curConfig ${CMAKE_CONFIGURATION_TYPES})
		string(TOLOWER ${curConfig} cfgLo)
		set(winCfgName "${winCfgName}$<$<CONFIG:${cfgLo}>:${curConfig}>")
	endforeach()
    set(winCfg "--config;${winCfgName}")
    
    if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
        set(lnxCfg "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}")
    endif()
    
	add_custom_command(TARGET ${target_name} POST_BUILD
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${name}"
			COMMAND ${CMAKE_COMMAND} ARGS ${lnxCfg} ${cmake_args} "${path}"
			COMMAND ${CMAKE_COMMAND} ARGS --build . ${winCfg} ${compile_args}
			COMMAND ${CMAKE_COMMAND} ARGS --build . ${winCfg} --target COPY_DEPENDENCIES
		)
endfunction()

# -----------------------------------------------------------------------------
function(_create_runner_script)
	if(NOT UNIX)
		message(FATAL_ERROR "Runner scripts are not implemented for windows")
	endif()

	# Create only when building an executable
	if(NOT ("${${PROJECT_NAME}_TARGET_TYPE}" STREQUAL EXECUTABLE))
		return()
	endif()

	set(RUNNER_NAME "run${${PROJECT_NAME}_TARGET_NAME}.sh")

	set(RUNNER_TMP "${CMAKE_BINARY_DIR}/CMakeFiles/${RUNNER_NAME}")
	configure_file("${SVS_CMAKE_HELPER_DIR}/templates/linux/runner.in" ${RUNNER_TMP})

	# Copying file is the only way to make it executable
	file(COPY ${RUNNER_TMP}
			DESTINATION "${CMAKE_BINARY_DIR}"
			FILE_PERMISSIONS
							OWNER_READ
							OWNER_EXECUTE
							GROUP_READ
							GROUP_EXECUTE
							WORLD_READ
							WORLD_EXECUTE
		)
	file(REMOVE ${RUNNER_TMP})

endfunction()

# -----------------------------------------------------------------------------
function(svs_add_vs_runtime_to_installer)
	if(NOT WIN32)
		message(FATAL_ERROR "Only Windows installers can distribute the Visual Studio runtime")
	endif()

	if (ARGC LESS 1)
		set(redistVsVersion "${SVS_IMPORTS_COMPILER}")
	else()
		set(redistVsVersion "${ARGV0}")
	endif()

	if (ARGC LESS 2)
		if	("${SVS_TARGET_ARCH}" STREQUAL "i386")
			set(redistArch "x86")
		else()
			set(redistArch "x64")
		endif()
	elseif(ARGC EQUAL 2)
		set(redistArch "${ARGV1}")
	else()
		message(FATAL_ERROR "Invalid usage of svs_add_vs_runtime_to_installer. Should be svs_add_vs_runtime_to_installer([vs_version [architecture]])")
	endif()

	set(runtimeDir "${IMPORTS_DIR}/Visual\ Studio\ Redistributables/${redistVsVersion}_${redistArch}")

	if (EXISTS "${runtimeDir}")
		file(GLOB globFiles "${runtimeDir}/*.exe")
		foreach(globFileName ${globFiles})
			message(STATUS "Adding runtime ${globFileName}") 
			svs_add_asset("${globFileName}" "../redistributables" INSTALL)
			get_filename_component(runtimeFileName "${globFileName}" NAME)
			file(READ "${runtimeDir}/Command\ Line.txt" runtimeCommandLine)
			set(SVS_VS_RUNTIME_COMMANDS "${SVS_VS_RUNTIME_COMMANDS}\\n\\tExecWait '\\\"$INSTDIR\\\\redistributables\\\\${runtimeFileName}\\\" ${runtimeCommandLine}'" PARENT_SCOPE)
		endforeach()
	endif()
endfunction()

# -----------------------------------------------------------------------------
function(svs_add_wibu_runtime_to_installer version)
	if (WIN32)
		if (NOT SVS_WIBU_ADDED_TO_INSTALLER)
			set(runtimeDir "${IMPORTS_DIR}/Wibu/${version}/installers")

			if (EXISTS "${runtimeDir}")		
				set(SVS_WIBU_ADDED_TO_INSTALLER true CACHE INTERNAL "")
				file(GLOB globFiles "${runtimeDir}/*.exe")
				foreach(globFileName ${globFiles})
					svs_add_asset("${globFileName}" "../redistributables" INSTALL)
					get_filename_component(runtimeFileName "${globFileName}" NAME)
					set(SVS_VS_RUNTIME_COMMANDS "${SVS_VS_RUNTIME_COMMANDS}\\n\\tExecWait '\\\"$INSTDIR\\\\redistributables\\\\${runtimeFileName}\\\"'" PARENT_SCOPE)
				endforeach()
			else()
				message(FATAL_ERROR "Could not locate installer for WIBU version ${version}")
			endif()
		endif()
	else()
		message("Only Windows installers can distribute the Wibu runtime")
	endif()
endfunction()
# -----------------------------------------------------------------------------
function (svs_installer_add_eula)	
	foreach(e ${ARGN})
		get_filename_component(e ${e} ABSOLUTE)
		list(APPEND eulaList "${e}")
	endforeach()
	
	set(${PROJECT_NAME}_EULA_LIST ${${PROJECT_NAME}_EULA_LIST} ${eulaList} PARENT_SCOPE)
endfunction()
# -----------------------------------------------------------------------------
function(svs_create_installer)	
   # Handle optional arguments
   set (options)
   set (oneValueArgs
         NAME
			VENDOR
         CONTACT
         SUMMARY
		)
	set (multiValueArgs)

	cmake_parse_arguments(SVS_INSTALLER "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
	
   if (NOT SVS_INSTALLER_VENDOR OR "${SVS_INSTALLER_VENDOR}" STREQUAL "")
		set(SVS_INSTALLER_VENDOR "Peter Plompen")
	endif()
   
   	if (NOT SVS_INSTALLER_CONTACT OR "${SVS_INSTALLER_CONTACT}" STREQUAL "")
		set(SVS_INSTALLER_CONTACT "${SVS_INSTALLER_VENDOR}")
	endif()
   
   	if (NOT SVS_INSTALLER_SUMMARY OR "${SVS_INSTALLER_SUMMARY}" STREQUAL "")
		set(SVS_INSTALLER_SUMMARY "${SVS_INSTALLER_VENDOR} - ${name}")
	endif()
   

   # Fill in name variable
   if (${PROJECT_NAME}_TARGET_NAME)
      if (SVS_INSTALLER_NAME AND NOT "${SVS_INSTALLER_NAME}" STREQUAL "${${PROJECT_NAME}_TARGET_NAME}")
         message(FATAL_ERROR "Name argument to svs_create_installer does not match argument set by svs_executable")
      endif()
      set(name ${${PROJECT_NAME}_TARGET_NAME})
   else()
      if (NOT SVS_INSTALLER_NAME)
         message(FATAL_ERROR "To use svs_create_installer without svs_executable, a name argument must be passed")
      endif() 
      set(name ${SVS_INSTALLER_NAME})
	endif()

   _ensure_install_directory_set()
   
   	# Setup the version information
	string(TOUPPER "${PROJECT_NAME}" upn)
	set(version "${SVS_${upn}_VERSION_MAJOR}.${SVS_${upn}_VERSION_MINOR}.${SVS_${upn}_VERSION_SUB}.${SVS_${upn}_VERSION_REVISION}")

   	# Handle the deprecated SVS_EULA_FILE variable
	if (NOT "${SVS_EULA_FILE}" STREQUAL "")
		message("The SVS_EULA_FILE variable has been deprecated. Please call the svs_installer_add_eula function instead.")
		svs_installer_add_eula("${SVS_EULA_FILE}")
	endif()
	
   	# Use the default EULA file if none has been provided
	if ("${PROJECT_NAME}_EULA_LIST" STREQUAL "")
		svs_installer_add_eula("${CMAKE_SOURCE_DIR}/Eula.txt")
	endif()

	set(CPACK_PACKAGE_VENDOR "${SVS_INSTALLER_VENDOR}")
	set(CPACK_PACKAGE_CONTACT "${SVS_INSTALLER_CONTACT}")
	set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "${SVS_INSTALLER_SUMMARY}")
	
	set(CPACK_PACKAGE_VERSION ${version})
	set(CPACK_PACKAGE_VERSION_MAJOR "${SVS_${upn}_VERSION_MAJOR}")
	set(CPACK_PACKAGE_VERSION_MINOR "${SVS_${upn}_VERSION_MINOR}")
	set(CPACK_PACKAGE_VERSION_PATCH "${SVS_${upn}_VERSION_SUB}")
	
	# Set the first item in the eula list to be the default
	#list(GET ${PROJECT_NAME}_EULA_LIST 0 CPACK_RESOURCE_FILE_LICENSE)

	if(WIN32)
		if("${CPACK_GENERATOR}" STREQUAL "")
			set(CPACK_GENERATOR NSIS)
		endif()
		
		if (("${SVS_INSTALL_ICON}" STREQUAL "") AND (EXISTS "${CMAKE_SOURCE_DIR}/Icon.ico"))
			set(SVS_INSTALL_ICON "${CMAKE_SOURCE_DIR}/Icon.ico")
		endif()
		
		# RelWithDebInfo is hardcoded as it's better to ship old version rather than a debug one
		install(DIRECTORY "${CMAKE_BINARY_DIR}/RelWithDebInfo/"
				DESTINATION "bin"
				CONFIGURATIONS RelWithDebInfo
				REGEX  ".pdb|.ilk|.exp|.lib$" EXCLUDE
			)

		if ("${CPACK_PACKAGE_NAME}" STREQUAL "")
			set(CPACK_PACKAGE_NAME "${name}")
		endif()

		if ("${CPACK_PACKAGE_INSTALL_DIRECTORY}" STREQUAL "")
         set(CPACK_PACKAGE_INSTALL_DIRECTORY "${SVS_INSTALL_DIR}")
		endif()
      
      # The "WIX_UI_BANNER" variable is actually used by both Wix and Nsis (bit of a hack, needed a 
      # pre-defined variable to carry through to the installer script)
      if (NOT CPACK_WIX_UI_BANNER OR "${CPACK_WIX_UI_BANNER}" STREQUAL "")
         set(CPACK_WIX_UI_BANNER "${SVS_CMAKE_HELPER_DIR}/templates/nsis/InstallationHeader.bmp")
      endif()
      
      string(REPLACE "/" "\\\\" CPACK_WIX_UI_BANNER "${CPACK_WIX_UI_BANNER}")
      
		if("${CPACK_GENERATOR}" STREQUAL "NSIS")      
			# NSIS requires flipping slashes in destination
         string(REPLACE "/" "\\\\" CPACK_PACKAGE_INSTALL_DIRECTORY "${CPACK_PACKAGE_INSTALL_DIRECTORY}")
		endif()
      
		if ("${CPACK_PACKAGE_INSTALL_REGISTRY_KEY}" STREQUAL "")
         string(REPLACE " " "" CPACK_PACKAGE_INSTALL_REGISTRY_KEY "Sentient_${name}_${SVS_IMPORTS_ARCH}")
		endif()
		
		set(CPACK_PROPERTIES_FILE "${CMAKE_BINARY_DIR}/SvsCPackProperties.cmake")
		file(WRITE "${CPACK_PROPERTIES_FILE}" "# SVS Installer Options")
		
		# Shortcut on desktop
		if(("${${PROJECT_NAME}_TARGET_TYPE}" STREQUAL EXECUTABLE) AND (NOT SVS_NO_DEFAULT_SHORTCUTS))
			set(CPACK_PACKAGE_EXECUTABLES "${name};${name}")
			set(CPACK_CREATE_DESKTOP_LINKS "${name};${name}")
		endif()
		
		if("${CPACK_GENERATOR}" STREQUAL "WIX")
			if (NOT "${SVS_INSTALL_ICON}" STREQUAL "")
				set(CPACK_WIX_PRODUCT_ICON "${SVS_INSTALL_ICON}")
			endif()
		else()
			if ("${CPACK_NSIS_COMPRESSOR}" STREQUAL "")
				set(CPACK_NSIS_COMPRESSOR "/SOLID lzma")
			endif()
			
			if ("${CPACK_NSIS_DISPLAY_NAME}" STREQUAL "")
				set(CPACK_NSIS_DISPLAY_NAME ${name})
			endif()
			
			set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS "${SVS_VS_RUNTIME_COMMANDS}\\n\\t${SVS_POST_INSTALL_COMMANDS}")
			set(CPACK_NSIS_EXTRA_UNINSTALL_COMMANDS "${SVS_POST_UNINSTALL_COMMANDS}")
			set(CPACK_NSIS_MUI_ICON "${SVS_INSTALL_ICON}")
			
			if ("${SVS_UNINSTALL_ICON}" STREQUAL "")
				set(CPACK_NSIS_MUI_UNIICON "${SVS_INSTALL_ICON}")
			else()
				set(CPACK_NSIS_MUI_UNIICON "${SVS_UNINSTALL_ICON}")
			endif()
			
			if (NOT "${CPACK_NSIS_MUI_UNIICON}" STREQUAL "")
				svs_add_asset("${CPACK_NSIS_MUI_UNIICON}" "../assets" INSTALL)
				get_filename_component(iconFileName "${CPACK_NSIS_MUI_UNIICON}" NAME)
				set(CPACK_NSIS_INSTALLED_ICON_NAME "assets\\\\${iconFileName}")
			endif()
			
			# Write out any optional additional EULA files
			list(LENGTH ${PROJECT_NAME}_EULA_LIST eulaCount)
			
			if (${eulaCount} GREATER 0)
				math(EXPR eulaCount ${eulaCount}-1)
			
				foreach(i RANGE 0 ${eulaCount})
					list(GET ${PROJECT_NAME}_EULA_LIST ${i} curEulaFile)
					string(REPLACE "/" "\\\\" curEulaFile "${curEulaFile}")
					set(additionalEulaList ${additionalEulaList} "\\n\\t!insertmacro MUI_PAGE_LICENSE \\\"${curEulaFile}\\\"")
				endforeach(i)
				
				file(APPEND "${CPACK_PROPERTIES_FILE}" "\nSET(SVS_ADDITIONAL_EULA_LIST \"${additionalEulaList}\")")			
			endif()
		endif()
	else()
		set(BIN_PRIVS OWNER_READ
					OWNER_EXECUTE
					GROUP_READ
					GROUP_EXECUTE
					WORLD_READ
					WORLD_EXECUTE
			)

		# Security check for not releasing a debug version
		if(NOT ("${CMAKE_BUILD_TYPE}" STREQUAL "Release"))
			message("\nNo install/package targets created\nInstaller available only in Release mode!\n")
			return()
		endif()

		if("${${PROJECT_NAME}_TARGET_TYPE}" STREQUAL EXECUTABLE)
			install(PROGRAMS "${CMAKE_BINARY_DIR}/bin/${name}"
					DESTINATION "${SVS_INSTALL_DIR}/bin"
				)
		endif()

		if(NOT ("${IMPORTS_DEPENDENCIES}" STREQUAL ""))
			install(DIRECTORY "${CMAKE_BINARY_DIR}/lib/"
					DESTINATION "${SVS_INSTALL_DIR}/lib"
					FILE_PERMISSIONS ${BIN_PRIVS}
				)
		endif()

		if("${CPACK_GENERATOR}" STREQUAL "")
			set(CPACK_GENERATOR DEB)
		endif()
		set(CPACK_PACKAGE_NAME "${name}")

		SET(CPACK_DEBIAN_PACKAGE_SECTION "video")
		SET(CPACK_RPM_PACKAGE_GROUP "video")
		SET(CPACK_RPM_PACKAGE_AUTOREQPROV "no")
		SET(CPACK_RPM_EXCLUDE_FROM_AUTO_FILELIST_ADDITION "/opt")
		if( SVS_TARGET_ARCH STREQUAL "armv7" )
			SET(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf" )
			MESSAGE( STATUS "Generating deb file with architecture of ${CPACK_DEBIAN_PACKAGE_ARCHITECTURE}" )
		else()
			MESSAGE( STATUS "Generating deb file with architecture of [CPACK DEFAULT]=${SVS_TARGET_ARCH}" )
		endif()

		# Include runner
		if("${${PROJECT_NAME}_TARGET_TYPE}" STREQUAL EXECUTABLE)
			set(RUNNER_NAME "run${name}.sh")
			install(FILES "${CMAKE_BINARY_DIR}/${RUNNER_NAME}"
					DESTINATION "${SVS_INSTALL_DIR}/"
					PERMISSIONS ${BIN_PRIVS}
				)
		endif()

		# Post install: global and custom
		set(SVS_PREINST_CONTENTS "${CMAKE_SOURCE_DIR}/preinst.in")
		if(EXISTS "${SVS_PREINST_CONTENTS}")
			file(READ "${SVS_PREINST_CONTENTS}" PREINST_CONTENT)
		else()
			set(PREINST_CONTENT "")
		endif()

		# License
		list(LENGTH ${PROJECT_NAME}_EULA_LIST eulaCount)
				
		if(EXISTS "${CPACK_RESOURCE_FILE_LICENSE}")
			file(READ "${CPACK_RESOURCE_FILE_LICENSE}" EULA_CONTENT)
			configure_file("${SVS_CMAKE_HELPER_DIR}/templates/linux/preinst_eula.in" "${CMAKE_BINARY_DIR}/preinst")
		else()
			configure_file("${SVS_CMAKE_HELPER_DIR}/templates/linux/preinst.in" "${CMAKE_BINARY_DIR}/preinst")
		endif()

		# Post install: global and custom
		set(SVS_POSTINST_CONTENTS "${CMAKE_SOURCE_DIR}/postinst.in")
		if(EXISTS "${SVS_POSTINST_CONTENTS}")
			file(READ "${SVS_POSTINST_CONTENTS}" POSTINST_CONTENT)
		else()
			set(POSTINST_CONTENT "")
		endif()

		configure_file("${SVS_CMAKE_HELPER_DIR}/templates/linux/postinst.in" "${CMAKE_BINARY_DIR}/postinst")

		# Post rm: global and custom
		set(SVS_POSTRM_CONTENTS "${CMAKE_SOURCE_DIR}/postrm.in")
		if(EXISTS "${SVS_POSTRM_CONTENTS}")
			file(READ "${SVS_POSTRM_CONTENTS}" POSTRM_CONTENT)
		else()
			set(POSTRM_CONTENT "")
		endif()

		configure_file("${SVS_CMAKE_HELPER_DIR}/templates/linux/postrm.in" "${CMAKE_BINARY_DIR}/postrm")

		SET(CPACK_RPM_PRE_INSTALL_SCRIPT_FILE "${CMAKE_BINARY_DIR}/preinst")
		SET(CPACK_RPM_POST_INSTALL_SCRIPT_FILE "${CMAKE_BINARY_DIR}/postinst")
		SET(CPACK_RPM_POST_REMOVE_SCRIPT_FILE "${CMAKE_BINARY_DIR}/postrm")

		SET(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_BINARY_DIR}/preinst;${CMAKE_BINARY_DIR}/postinst;${CMAKE_BINARY_DIR}/postrm")

		# Experimental! Remove and set manually if dependency checking does not work
		if (CMAKE_CROSSCOMPILING)
			gcc_version_cross(gcc)
			glibc_version_cross(glibc)
		else()
			gcc_version(gcc)
			glibc_version(glibc)
		endif()
		SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= ${glibc}), libgcc1 (>= 1:${gcc}), libstdc++6 (>= ${gcc})")
		# SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.19), libgcc1 (>= 1:4.9.2), libstdc++6 (>= 4.9.2)")
	endif()

	include(CPack)
endfunction()

# -----------------------------------------------------------------------------
function(_add_cross_compile_copy_libs_target binary_name)
	#Add a target to copy all .so files to a folder on the host
	add_custom_target(CROSS_COMPILE_COPY_LIB)
	add_dependencies(CROSS_COMPILE_COPY_LIB ${binary_name})
	set(binary "${CMAKE_BINARY_DIR}/bin/${binary_name}")

	# Get list ready to pass as argument
	get_property(LIB_DIRECTORIES GLOBAL PROPERTY GLOBAL_LIBS_TO_COPY)
	set(libdirs "emptyDir")
	foreach(libdir ${LIB_DIRECTORIES})
		set(libdirs "${libdirs}\\;${libdir}")
	endforeach(libdir)

	# Copy libs for all copy targets
	foreach(SVS_COPY_TARGET ${SVS_COPY_TARGETS})
		# Run the rest after building - need the binary!
		add_custom_command(TARGET CROSS_COMPILE_COPY_LIB POST_BUILD
				COMMAND ${CMAKE_COMMAND} ARGS
						-DBIN="${binary}"
						-DLIBS="${libdirs}"
                        -DCOPY_TARGET="${SVS_COPY_TARGET}"
                        -DSVS_INSTALL_DIR="${SVS_INSTALL_DIR}"
						-DLDD="${SVS_LDD}"
						-DIMPORTS_DIR="${IMPORTS_DIR}"
						-P "${SVS_CMAKE_HELPER_DIR}/svs_cross_copylibs.cmake"
			)
	endforeach()

endfunction()

# -----------------------------------------------------------------------------
function(_add_cross_compile_copy_bin_target binary_name)
	#Copy the binary to the remote system
	add_custom_target(CROSS_COMPILE_COPY_BIN)
	add_dependencies(CROSS_COMPILE_COPY_BIN ${binary_name})
	set(binary "${CMAKE_BINARY_DIR}/bin/${binary_name}")
   _create_runner_script()
   
   if (NOT ${PROJECT_NAME}_TARGET_NAME)
      message(FATAL_ERROR "Attempt to create CROSS_COMPILE_COPY_BIN but svs_executable not called")
   endif()

	foreach(SVS_COPY_TARGET ${SVS_COPY_TARGETS})
		add_custom_command(TARGET CROSS_COMPILE_COPY_BIN POST_BUILD
			COMMAND echo "Copying binaries to ${SVS_COPY_TARGET}:${SVS_INSTALL_DIR}/bin/..."
			VERBATIM
			)
		add_custom_command(TARGET CROSS_COMPILE_COPY_BIN POST_BUILD
				COMMAND ssh ${SVS_COPY_TARGET} mkdir -p ${SVS_INSTALL_DIR}
				VERBATIM
			)
		add_custom_command(TARGET CROSS_COMPILE_COPY_BIN POST_BUILD
				COMMAND ssh ${SVS_COPY_TARGET} mkdir -p ${SVS_INSTALL_DIR}/bin
				VERBATIM
			)
		add_custom_command(TARGET CROSS_COMPILE_COPY_BIN POST_BUILD
				COMMAND scp ${binary} ${SVS_COPY_TARGET}:${SVS_INSTALL_DIR}/bin/
				VERBATIM
			)

		# Copy runner
		set(RUNNER_NAME "run${${PROJECT_NAME}_TARGET_NAME}.sh")
		add_custom_command(TARGET CROSS_COMPILE_COPY_BIN POST_BUILD
				COMMAND scp ${CMAKE_BINARY_DIR}/${RUNNER_NAME} ${SVS_COPY_TARGET}:~/${binary_name}/
				VERBATIM
			)
	endforeach(SVS_COPY_TARGET)

endfunction()
