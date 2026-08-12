# -----------------------------------------------------------------------------
function(svs_svslib_path path)
    message(FATAL_ERROR "svs_svslib_path is deprecated")
endfunction()

function(svs_imports_path arg)
    message(FATAL_ERROR "svs_imports_path is deprecated")
endfunction()

# -----------------------------------------------------------------------------
function(_init_svs_helpers path)
    # Note: As CMake essentially inlines its function calls, CMAKE_CURRENT_LIST_DIR
    # is the directory of the file that called the function, not the directory in
    # which svs_helpers script resides.
    set(SVS_CMAKE_HELPER_DIR ${path} CACHE PATH "")

endfunction()

# -----------------------------------------------------------------------------
function(svs_init imports_path svs_library_path)
   
   # Allow a global Imports override based on environment variables
    if(DEFINED ENV{SVS_IMPORTS_OVERRIDE_DIR})
        set(imports_path $ENV{SVS_IMPORTS_OVERRIDE_DIR})
        message("Forcing imports dir to ${imports_path}")
    endif()
   
    # Set default variables   
    if (NOT SVS_TOP_LEVEL_PROJECT) # Store the first project name that we encounter
        set(SVS_TOP_LEVEL_PROJECT ${PROJECT_NAME} CACHE STRING "")
    endif()

    # Tell VS to keep CMake stuff in the CMake folder (filter)
    if(DEFINED MSVC)
        set(PREDEFINED_TARGETS_FOLDER "CMake" CACHE STRING "")
        set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
        set_property(GLOBAL PROPERTY USE_FOLDERS on)
    endif()

    # Export include paths etc for intellisense on Linux
    set(CMAKE_EXPORT_COMPILE_COMMANDS "TRUE" CACHE BOOL "" FORCE)

    # Set project name
    if(ALL_${PROJECT_NAME}_SOURCES)
        message(FATAL_ERROR "Project name " ${PROJECT_NAME} " is already used. "
                "CMake doesn't mind it but SVS helpers do. "
                "Please rename it or figure out why is it included twice.")
    else()
        set(ALL_${PROJECT_NAME}_SOURCES "" PARENT_SCOPE) # Don't you dare change it to CACHE
    endif()

    # Reset import dependencies
    set(IMPORTS_DEPENDENCIES "" PARENT_SCOPE)

    # Set SVSLibrary path
    if(NOT SVSLIB_DIR)
        if(NOT IS_ABSOLUTE "${svs_library_path}")
            set(svs_library_path "${CMAKE_SOURCE_DIR}/${svs_library_path}")
        endif()
        get_filename_component(svs_library_path "${svs_library_path}" ABSOLUTE)

        message(STATUS "* SVS lib search path TEST: " ${svs_library_path})
        set(SVSLIB_DIR ${svs_library_path} CACHE PATH "")
    endif()

    include_directories(${SVSLIB_DIR})
    include_directories(${SVSLIB_DIR}/..)

    _svs_set_arch()
    _svs_set_compiler()


    # Set Imports_v2 location
    if(NOT IMPORTS_DIR)
        if(NOT IS_ABSOLUTE "${imports_path}")
            set(imports_path "${CMAKE_SOURCE_DIR}/${imports_path}")
            get_filename_component(IMPORTS_DIR "${imports_path}" ABSOLUTE)

            message(STATUS "* Imports search path: " ${IMPORTS_DIR})
        else()
            set(IMPORTS_DIR "${imports_path}")
            message("* Imports search path (absolute): " ${IMPORTS_DIR})
        endif()

        set(IMPORTS_DIR "${IMPORTS_DIR}" CACHE PATH "" FORCE)
    else()
        if(NOT IS_ABSOLUTE "${IMPORTS_DIR}")
            set(IMPORTS_DIR "${CMAKE_SOURCE_DIR}/${IMPORTS_DIR}")
            get_filename_component(IMPORTS_DIR "${IMPORTS_DIR}" ABSOLUTE)
            message(STATUS "* Forced imports search path: " ${IMPORTS_DIR})

            set(IMPORTS_DIR "${IMPORTS_DIR}" CACHE PATH "" FORCE)
        endif()
    endif()


    # Set output directory for all subproject
    if(WIN32)
        if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG)
            foreach(curConfig ${CMAKE_CONFIGURATION_TYPES})
                string(TOUPPER ${curConfig} cfgUp)
                set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgUp} "${CMAKE_BINARY_DIR}/${curConfig}" CACHE INTERNAL "")
            endforeach()
            #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_BINARY_DIR}/Debug"          CACHE INTERNAL "")
            #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_BINARY_DIR}/Release"        CACHE INTERNAL "")
            #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL     "${CMAKE_BINARY_DIR}/MinSizeRel"     CACHE INTERNAL "")
            #set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}/RelWithDebInfo" CACHE INTERNAL "")
        endif()
    else()
        if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
            set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin" CACHE INTERNAL "")
            set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib" CACHE INTERNAL "")
        endif()
    endif()

    if(WIN32)
        set(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo" CACHE INTERNAL "")
        _vs_set_working_directory()
    endif()

    # Add dev name macro
    if(DEFINED ENV{DEV_NAME})
        set(dev_name $ENV{DEV_NAME})
        add_definitions(-D${dev_name}=1)
    endif()

    # Bounds checking definintions
    if (BOUNDS_CHECKING_MICROSOFT_GSL OR BOUNDS_CHECKING_SVS_SAFEPOINTER OR BOUNDS_CHECKING_PARTIAL_GSL_LITE)
        if (BOUNDS_CHECKING_OFF)
            message(FATAL_ERROR "Bounds checking is turned off but you have selected a bounds checking method")
        endif()
        if (BOUNDS_CHECKING_MICROSOFT_GSL)
            add_definitions(-DUSE_MICROSOFT_GSL)
            add_definitions(-DSVS_BREAKPOINTS_FOR_RELEASE_STAND_ALONE)
            svs_import(HEADERS gsl 1.0.0)
        elseif (BOUNDS_CHECKING_SVS_SAFEPOINTER)
            add_definitions(-DSAFETY_CHECK_MEMORY_POINTERS)
            add_definitions(-DSVS_BREAKPOINTS_FOR_RELEASE_STAND_ALONE)
        elseif (BOUNDS_CHECKING_PARTIAL_GSL_LITE)
            add_definitions(-DUSE_GSL_LITE)
            add_definitions(-DSVS_BREAKPOINTS_FOR_RELEASE_STAND_ALONE)
            svs_import(HEADERS gsl-lite 0.32.0)
        endif()
    else()
        # No bounds checking - this should be the default.
    endif()

endfunction()

# -----------------------------------------------------------------------------
function(_vs_get_bin_dir varName)
    if(NOT MSVC)
        message(FATAL_ERROR "_vs_get_bin_dir: Trying to use VS string for non VS environment")
    endif()

    foreach(curConfig ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${curConfig} cfgUp)
        string(TOLOWER ${curConfig} cfgLo)
        set(ret "${ret}$<$<CONFIG:${cfgLo}>:${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgUp}}>")
    endforeach()

    set("${varName}" "${ret}" PARENT_SCOPE)

endfunction()

# -----------------------------------------------------------------------------
function(svs_version major minor sub)
    # Revision can be overriden by specifying a fourth parameter

    string(TOUPPER "${PROJECT_NAME}" upn)
    
    set(rev 0)
    set(branch "-")

    if(${ARGC} GREATER 3)
        set(rev ${ARGV3})
        message(STATUS "Forcing revision as " ${rev})
    else()
        _get_repository_revision(rev branch)

        if("${rev}" STREQUAL "")
            message("Error setting ${PROJECT_NAME} SVN revision number: ${err}")
            set(rev 0)
        endif()

        # message(STATUS "* Setting ${PROJECT_NAME} revision: ${rev}, branch ${branch}")
    endif()

    message(STATUS "${upn} ver ${major}.${minor}.${sub} (${rev} ${branch})")
    set("SVS_${upn}_VERSION_MAJOR"    "${major}" CACHE INTERNAL "")
    set("SVS_${upn}_VERSION_MINOR"    "${minor}" CACHE INTERNAL "")
    set("SVS_${upn}_VERSION_SUB"      "${sub}"   CACHE INTERNAL "")
    set("SVS_${upn}_VERSION_REVISION" "${rev}"   CACHE INTERNAL "")
    
    configure_file("${SVS_CMAKE_HELPER_DIR}/templates/version_cpp/version_cpp.in"
                    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}_version.cpp")

    configure_file("${SVS_CMAKE_HELPER_DIR}/templates/version_cpp/version_h.in"
                    "${CMAKE_BINARY_DIR}/${PROJECT_NAME}_version.h")

    set(${PROJECT_NAME}_VERSION_DONE yes CACHE INTERNAL "") # Mark as done

endfunction()

# -----------------------------------------------------------------------------
function(_get_repository_revision varRev varBranch)
    if(UNIX)
        set(cmd "svn info | awk '/Revision/{printf $2}'")
        execute_process(COMMAND bash "-c" ${cmd}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE rev
                        ERROR_VARIABLE err)

        set(cmd "svn info | awk '/Relative URL/{match($3, \"(trunk|branches|tags)/(.+)\", a)} END{if(a[1] == \"trunk\"){printf \"trunk\"}else{printf a[2]}}'")
        execute_process(COMMAND bash "-c" ${cmd}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE branch
                        ERROR_VARIABLE err)
    else()
        set(cmd "&svn info | Select-String -Pattern \"Revision.* (\\d+)$\" | % {[Console]::Out.Write(\"$($_.matches.groups[1])\")}")
        execute_process(COMMAND powershell -Command ${cmd}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE rev
                        ERROR_VARIABLE err)

        set(cmd "&svn info | Select-String -Pattern \"Relative URL:\\s\\^.+(trunk|branches|tags)/(.+)$\" | % {if(\"$($_.matches.groups[1])\" -eq \"trunk\"){[Console]::Out.Write(\"trunk\")}else{[Console]::Out.Write(\"$($_.matches.groups[2])\")}}")
        execute_process(COMMAND powershell -Command ${cmd}
                        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                        OUTPUT_VARIABLE branch
                        ERROR_VARIABLE err)
    endif()

    if("${rev}" STREQUAL "")
        # Try git... TODO fix me when/if we migrate
        message(STATUS "SVN version checking failed, trying git")

        if(UNIX)
            set(cmd "git svn info | awk '/Revision/{printf $2}'")
            execute_process(COMMAND bash "-c" ${cmd}
                            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                            OUTPUT_VARIABLE rev
                            ERROR_VARIABLE giterr)
        else()
            set(cmd "&git svn info | Select-String -Pattern \"Revision.* (\\d+)$\" | % {\"$($_.matches.groups[1])\"}")
            execute_process(COMMAND powershell -Command ${cmd}
                            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                            OUTPUT_VARIABLE rev
                            ERROR_VARIABLE giterr)
        endif()
    endif()

    set(${varRev}    ${rev}    PARENT_SCOPE)
    set(${varBranch} ${branch} PARENT_SCOPE)

endfunction()

# -----------------------------------------------------------------------------
# Modified version of generate_product_version from https://github.com/halex2005/CMakeHelpers
function(svs_create_win_versioninfo outfiles)
    message(STATUS "Adding VersionInfo resources to ${PROJECT_NAME}")
    
    string(TOUPPER "${PROJECT_NAME}" SVS_UPPER_PROJECT_NAME)
    
    set (options)
    set (oneValueArgs
            NAME
            BUNDLE
            VERSION_MAJOR
            VERSION_MINOR
            VERSION_PATCH
            VERSION_REVISION
            COMPANY_NAME
            COMPANY_COPYRIGHT
            COMMENTS
            ORIGINAL_FILENAME
            INTERNAL_NAME
            FILE_DESCRIPTION
            ICON
        )
    set (multiValueArgs)
        
    cmake_parse_arguments(SVS_PRODUCT "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    if (NOT SVS_PRODUCT_NAME OR "${SVS_PRODUCT_NAME}" STREQUAL "")
        set(SVS_PRODUCT_NAME "${PROJECT_NAME}")
    endif()
    
    if (NOT SVS_PRODUCT_BUNDLE OR "${SVS_PRODUCT_BUNDLE}" STREQUAL "")
        set(SVS_PRODUCT_BUNDLE "${SVS_PRODUCT_NAME}")
    endif()
    
    if (NOT SVS_PRODUCT_VERSION_MAJOR OR "${SVS_PRODUCT_VERSION_MAJOR}" STREQUAL "")
        set(SVS_PRODUCT_VERSION_MAJOR "${SVS_${SVS_UPPER_PROJECT_NAME}_VERSION_MAJOR}")
    endif()
    if (NOT SVS_PRODUCT_VERSION_MINOR OR "${SVS_PRODUCT_VERSION_MINOR}" STREQUAL "")
        set(SVS_PRODUCT_VERSION_MINOR "${SVS_${SVS_UPPER_PROJECT_NAME}_VERSION_MINOR}")
    endif()
    if (NOT SVS_PRODUCT_VERSION_PATCH OR "${SVS_PRODUCT_VERSION_PATCH}" STREQUAL "")
        set(SVS_PRODUCT_VERSION_PATCH "${SVS_${SVS_UPPER_PROJECT_NAME}_VERSION_SUB}")
    endif()
    if (NOT SVS_PRODUCT_VERSION_REVISION OR "${SVS_PRODUCT_VERSION_REVISION}" STREQUAL "")
        set(SVS_PRODUCT_VERSION_REVISION "${SVS_${SVS_UPPER_PROJECT_NAME}_VERSION_REVISION}")
    endif()
    
    if (NOT SVS_PRODUCT_COMPANY_NAME OR "${SVS_PRODUCT_COMPANY_NAME}" STREQUAL "")
        set(SVS_PRODUCT_COMPANY_NAME "Peter Plompen Software")
    endif()
    if (NOT SVS_PRODUCT_COMPANY_COPYRIGHT OR "${SVS_PRODUCT_COMPANY_COPYRIGHT}" STREQUAL "")
        string(TIMESTAMP SVS_PRODUCT_CURRENT_YEAR "%Y")
        set(SVS_PRODUCT_COMPANY_COPYRIGHT "Copyright ${SVS_PRODUCT_CURRENT_YEAR} ${SVS_PRODUCT_COMPANY_NAME}")
    endif()
    if (NOT SVS_PRODUCT_COMMENTS OR "${SVS_PRODUCT_COMMENTS}" STREQUAL "")
        set(SVS_PRODUCT_COMMENTS "${SVS_PRODUCT_NAME} v${SVS_PRODUCT_VERSION_MAJOR}.${SVS_PRODUCT_VERSION_MINOR}")
    endif()
    if (NOT SVS_PRODUCT_ORIGINAL_FILENAME OR "${SVS_PRODUCT_ORIGINAL_FILENAME}" STREQUAL "")
        set(SVS_PRODUCT_ORIGINAL_FILENAME "${SVS_PRODUCT_NAME}")
    endif()
    if (NOT SVS_PRODUCT_INTERNAL_NAME OR "${SVS_PRODUCT_INTERNAL_NAME}" STREQUAL "")
        set(SVS_PRODUCT_INTERNAL_NAME "${SVS_PRODUCT_NAME}")
    endif()
    if (NOT SVS_PRODUCT_FILE_DESCRIPTION OR "${SVS_PRODUCT_FILE_DESCRIPTION}" STREQUAL "")
        set(SVS_PRODUCT_FILE_DESCRIPTION "${SVS_PRODUCT_NAME}")
    endif()
    
    if (NOT SVS_PRODUCT_ICON OR "${SVS_PRODUCT_ICON}" STREQUAL "")
      set(SVS_ICON_DEFINE " ")
        #set(SVS_HAS_PRODUCT_ICON "0")
    else()
      set(SVS_ICON_DEFINE "#define SVS_${SVS_UPPER_PROJECT_NAME}_HAS_ICON")
        #set(SVS_HAS_PRODUCT_ICON "1")
        get_filename_component(SVS_PRODUCT_ICON "${SVS_PRODUCT_ICON}" ABSOLUTE)
    endif()
    
    set (SVS_PRODUCT_VERSIONINFO_HEADER_FILE "${PROJECT_NAME}VersionInfo.h")    
    set (_VersionInfoFile "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}VersionInfo.h")
    set (_VersionResourceFile "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}VersionResource.rc")
    
    configure_file(
            "${SVS_CMAKE_HELPER_DIR}/templates/windows/VersionInfo.in"
            "${_VersionInfoFile}"
            @ONLY
        )

    configure_file(
            "${SVS_CMAKE_HELPER_DIR}/templates/windows/VersionResource.in"
            "${_VersionResourceFile}"
            @ONLY
        )

    svs_src(
            "VersionInfo\\"
            "${_VersionInfoFile}"
            "${_VersionResourceFile}"
        )

    unset(${outfiles})
    list(APPEND ${outfiles} "${_VersionInfoFile}" "${_VersionResourceFile}")
    
    unset(${outfiles} PARENT_SCOPE)
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)

endfunction()

# -----------------------------------------------------------------------------
function(_svs_set_compiler)
    # if(SVS_IMPORTS_COMPILER)
    #     return()
    # endif()

    if(DEFINED UNIX)
        if (CMAKE_CROSSCOMPILING)
            gcc_version_cross(gcc)
            glibc_version_cross(glibc)
        else()
            gcc_version(gcc)
            glibc_version(glibc)
        endif()

        if("${SVS_TARGET_ARCH}" STREQUAL "armv7")
            set(SVS_IMPORTS_COMPILER "linux_armv7l" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "linux_armv7l-${gcc}-${glibc}" CACHE INTERNAL "")
        elseif("${SVS_TARGET_ARCH}" STREQUAL "armv8")
            set(SVS_IMPORTS_COMPILER "linux_armv8" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "linux_armv8-${gcc}-${glibc}" CACHE INTERNAL "")
        elseif("${SVS_TARGET_ARCH}" STREQUAL "i386") # I.e. 32 bits
            set(SVS_IMPORTS_COMPILER "linux_i586" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "linux_i586-${gcc}-${glibc}" CACHE INTERNAL "")
        elseif("${SVS_TARGET_ARCH}" STREQUAL "x86_64")
            set(SVS_IMPORTS_COMPILER "linux_x86_64" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "linux_x86_64-${gcc}-${glibc}" CACHE INTERNAL "")
        else()
            message(FATAL_ERROR "Unknown architecture: ${SVS_TARGET_ARCH}")
        endif()

    else()
        if(DEFINED MSVC14)
            set(SVS_IMPORTS_COMPILER "vs2015" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "vs2015" CACHE INTERNAL "")
        elseif(DEFINED MSVC12)
            set(SVS_IMPORTS_COMPILER "vs2013" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "vs2013" CACHE INTERNAL "")
        elseif(DEFINED MSVC7)
            set(SVS_IMPORTS_COMPILER "vs2008" CACHE INTERNAL "")
            set(SVS_IMPORTS_COMPILER_FULL "vs2008" CACHE INTERNAL "")
        else()
            message(FATAL_ERROR "Unknown architecture: ${SVS_TARGET_ARCH}")
        endif()
    endif()

    message(STATUS "* Detected compiler: ${SVS_IMPORTS_COMPILER} (${SVS_TARGET_ARCH})")
    
endfunction()

# -----------------------------------------------------------------------------
function(_svs_set_arch)
    # if(SVS_IMPORTS_ARCH)
    #     return()
    # endif()

    # Code doesn't support PowerPC architecture, fix it if ever needed
    if(NOT SVS_TARGET_ARCH)
        target_architecture(SVS_TARGET_ARCH)
        set(SVS_TARGET_ARCH ${SVS_TARGET_ARCH} CACHE INTERNAL "")

        # Pass target architecture to C++ as a define (mainly to check if arm)
        add_definitions(-DSVS_TARGET_ARCH="${SVS_TARGET_ARCH}")

        # Pass target architecture as integer (32/64) for comparing
        if("${SVS_TARGET_ARCH}" STREQUAL "i386" OR "${SVS_TARGET_ARCH}" STREQUAL "armv7")
            add_definitions(-DSVS_TARGET_BITS=32)
        else()
            add_definitions(-DSVS_TARGET_BITS=64)
        endif()
    endif()

    if(UNIX)
        set(SVS_IMPORTS_ARCH "." CACHE INTERNAL "") # Included in compiler dir name
    else()
        # Windows
        if("${SVS_TARGET_ARCH}" STREQUAL "i386")
            set(SVS_IMPORTS_ARCH "win32" CACHE INTERNAL "")
        else()
            set(SVS_IMPORTS_ARCH "x64" CACHE INTERNAL "")
        endif()
    endif()

    message(STATUS "* Detected architecture: ${SVS_IMPORTS_ARCH} (${SVS_TARGET_ARCH})")

endfunction()

# -----------------------------------------------------------------------------
function(svs_add_flag platform stage flag)
    if(NOT ${platform})
        return()
    endif()

    if("${stage}" STREQUAL "LINK")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${flag}")
        set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS} ${flag}")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${flag}")
    elseif("${stage}" STREQUAL "LINK_DEBUG")
        set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG} ${flag}")
        set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG} ${flag}")
        set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG} ${flag}")
    elseif("${stage}" STREQUAL "LINK_RELEASE")
        set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE} ${flag}")
        set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE} ${flag}")
        set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE} ${flag}")
    elseif("${stage}" STREQUAL "COMPILE")
        set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} ${flag}")
    elseif("${stage}" STREQUAL "COMPILE_DEBUG")
        set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} ${flag}")
    elseif("${stage}" STREQUAL "COMPILE_RELEASE")
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${flag}")
    else()
        message(FATAL_ERROR "No such stage: ${stage}")
    endif()

    # Export the setting to parent scope
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}"         PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}"   PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_replace_flag platform stage flag_what flag_with)
    if(NOT ${platform})
        return()
    endif()

    if("${stage}" STREQUAL "LINK")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
    elseif("${stage}" STREQUAL "LINK_DEBUG")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
    elseif("${stage}" STREQUAL "LINK_RELEASE")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}")
    elseif("${stage}" STREQUAL "LINK_ALL")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}")
    elseif("${stage}" STREQUAL "COMPILE")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    elseif("${stage}" STREQUAL "COMPILE_DEBUG")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
    elseif("${stage}" STREQUAL "COMPILE_RELEASE")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    elseif("${stage}" STREQUAL "COMPILE_ALL")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}")
        string(REPLACE "${flag_what}" "${flag_with}" CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
    else()
        message(FATAL_ERROR "No such stage: ${stage}")
    endif()

    # Export the setting to parent scope
    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}"         PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}"   PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function (svs_set_default_flags)
    # Global defines
    add_definitions(
            -DCMAKE_REMOVE_DEPRECATED_CODE
        )

    if(WIN32)
        add_definitions(
                -D_WINSOCK_DEPRECATED_NO_WARNINGS
                -D_SCL_SECURE_NO_WARNINGS
                -D_CRT_SECURE_NO_WARNINGS
                -D_CRT_SECURE_NO_DEPRECATE
                -D_WIN32_WINNT=0x0501
                -DUNICODE
                -D_UNICODE
                -DNOMINMAX=1
            )

        set(VS_MP no CACHE BOOL "")
        if(${VS_MP})
            svs_add_flag(WIN32 COMPILE "/MP")              # Parallel compilation
        endif()

        set(IGNORE_WARNINGS no CACHE BOOL "")
        if(NOT ${IGNORE_WARNINGS})
            svs_add_flag(WIN32 COMPILE "/WX")              # Treat warnings as errors
        endif()

        svs_replace_flag(WIN32 COMPILE_ALL "/W3" "/W4")    # Increase warnings level
        svs_add_flag(WIN32 COMPILE_RELEASE "/GL")          # Global optimization

        # svs_add_flag(WIN32 COMPILE "/IGNORE:4100")       # Disable unreferenced parameter
        # svs_add_flag(WIN32 LINK "/WX")                   # Treat linking warnings as errors

        svs_add_flag(WIN32 LINK_RELEASE "/LTCG")           # Global linking optimization (release only)
        svs_add_flag(WIN32 LINK "/IGNORE:4099")            # Ignore missing .pdb

    else()
        add_definitions(-D_UNIX)

        if(NOT "${DONT_HIDE_SYMBOLS}")
            svs_add_flag(UNIX COMPILE "-fvisibility=hidden") # Hiding not exported symbols
        else()
            message("***** DONT_HIDE_SYMBOLS is defined, Why? We should set visibilty to hidden for everything we expose to customers *****")
        endif()

        svs_add_flag(UNIX COMPILE "-fpermissive")           # Disable this as soon as possible
        svs_add_flag(UNIX COMPILE "-fPIC")                  # Position independent code

        svs_add_flag(UNIX LINK "-Wl,--no-as-needed")

        # set(SVS_GCC_VERSION "${res}" CACHE INTERNAL "")
        set(CMAKE_CXX_EXTENSIONS off PARENT_SCOPE) 
        # Ninja does not pass on the colour output of GCC by default
        if(CMAKE_GENERATOR STREQUAL "Ninja")
            add_compile_options (-fdiagnostics-color=always)
        endif()
        if(SVS_GCC_MAJOR_VERSION GREATER 4)         
            if(CMAKE_VERSION VERSION_LESS "3.1")
            svs_add_flag(UNIX COMPILE "--std=c++14") 
            else()
            set(CMAKE_CXX_STANDARD 14 PARENT_SCOPE) # C++ 14 standard
            endif()
        else()
            if(CMAKE_VERSION VERSION_LESS "3.1")
            svs_add_flag(UNIX COMPILE "--std=c++11") 
            else()
            set(CMAKE_CXX_STANDARD 11 PARENT_SCOPE) # C++ 11 standard
            endif()
        endif()
        message(STATUS)
        svs_add_flag(UNIX COMPILE "-pthread")               # Smart linking to ptrhread (notice, it's not -lpthread)

        svs_add_flag(UNIX COMPILE "-Wno-write-strings")     # Disable warnings about char *s = "something";
        # svs_add_flag(UNIX COMPILE "-Wno-deprecated-declarations")

        if("${SVS_TARGET_ARCH}" STREQUAL "armv7")
            add_definitions(
                    -DARM_BUILD
                    -DUSE_SIMD_OPTIMISATION
                )
            svs_add_flag(UNIX COMPILE "-march=armv7")       # Platform specific optimizations
            svs_add_flag(UNIX COMPILE "-mfpu=neon-vfpv4")   # Even more optimized optimizations
        elseif("${SVS_TARGET_ARCH}" STREQUAL "armv8") # Arm 64
            add_definitions(
                    -DARM_BUILD
                    -DUSE_SIMD_OPTIMISATION
                )
         svs_add_flag(UNIX COMPILE "-march=armv8-a")     # Platform specific optimizations
        else()
            svs_add_flag(UNIX COMPILE "-march=core2")       # Platform specific optimizations - minium supported CPU is core2
        endif()
    endif()

    set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS}"         PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG}"   PARENT_SCOPE)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS    "${CMAKE_EXE_LINKER_FLAGS}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS_DEBUG "${CMAKE_SHARED_LINKER_FLAGS_DEBUG}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG    "${CMAKE_EXE_LINKER_FLAGS_DEBUG}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS_DEBUG "${CMAKE_MODULE_LINKER_FLAGS_DEBUG}" PARENT_SCOPE)
    set(CMAKE_SHARED_LINKER_FLAGS_RELEASE "${CMAKE_SHARED_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)
    set(CMAKE_EXE_LINKER_FLAGS_RELEASE    "${CMAKE_EXE_LINKER_FLAGS_RELEASE}"    PARENT_SCOPE)
    set(CMAKE_MODULE_LINKER_FLAGS_RELEASE "${CMAKE_MODULE_LINKER_FLAGS_RELEASE}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_set_configurations)
    # If no configuration is specified, surpress warning
    if(${ARGC} EQUAL 0)
        set(CONFIG_DONE yes CACHE BOOL "")
        return()
    endif()

    # Set preprocessor defines every time we regenerate solution
    foreach(c ${ARGV})
        if(${${c}})
            add_definitions("-D${c}=1")
        endif()
    endforeach()

    set(CONFIG_DONE yes CACHE BOOL "")

    set(cnt 0)

    message(STATUS "* Declared configurations:")
    foreach(c ${ARGV})
        set(${c} no CACHE BOOL "") # It won't overwrite already set value
        message(STATUS "*    ${c} = ${${c}}")
        if(${${c}})
            math(EXPR cnt "${cnt} + 1")
        endif()
    endforeach()

    if(NOT(${cnt} EQUAL 1))
        message(FATAL_ERROR "One and only one configuration must be specified. Specified configurations: ${cnt}\n\nYOU MAY NEED TO DELETE YOUR BUILD FOLDER (if configurations have changed in your cmake files)")
    endif()
endfunction()

# -----------------------------------------------------------------------------
function(_validate_platform where what)
    set(cnt 0)
    string(REPLACE " " ";" wlist ${what})

    foreach(w ${wlist})
        list(FIND where ${w} out)

        if(NOT(${out} EQUAL -1))
            math(EXPR cnt "${cnt} + 1")
            message(STATUS "* Platform matched: ${w}")
        endif()
    endforeach()

    if(${cnt} EQUAL 0)
        message(FATAL_ERROR "Platform is not supported!")
    endif()
endfunction()

# -----------------------------------------------------------------------------
function(svs_restrict_platforms)
    if(PLATFORM_VALIDATE_DONE)
        return()
    endif()

    if("${SVS_IMPORTS_COMPILER}" STREQUAL "armv7")
        _validate_platform("${ARGV}" "arm arm_32")
    elseif("${SVS_IMPORTS_COMPILER}" STREQUAL "linux_i586")
        _validate_platform("${ARGV}" "linux linux_32")
    elseif("${SVS_IMPORTS_COMPILER}" STREQUAL "linux_x86_64")
        _validate_platform("${ARGV}" "linux linux_64")
    else() # Visual Studio
        if("${SVS_TARGET_ARCH}" STREQUAL "i386")
            _validate_platform("${ARGV}" "${SVS_IMPORTS_COMPILER} ${SVS_IMPORTS_COMPILER}_32 win win_32")
        else()
            _validate_platform("${ARGV}" "${SVS_IMPORTS_COMPILER} ${SVS_IMPORTS_COMPILER}_64 win win_64")
        endif()
    endif()

    set(PLATFORM_VALIDATE_DONE yes CACHE INTERNAL "")
endfunction()

# -----------------------------------------------------------------------------
function(_get_last_path_elem path varName) # sap_ prefix to keep it unique...
    # Get the last part of the path
    string(REPLACE "/" ";" name ${path})
    list(LENGTH name len)
    math(EXPR len "${len} - 1")
    list(GET name ${len} name)

    set(${varName} "${name}" PARENT_SCOPE)
endfunction()

# -----------------------------------------------------------------------------
function(svs_add_subproject sap_location) # sap_ prefix to keep it unique...
    # This does not support including two project with the same name.
    # If you need to do so, include the second one manually specifying different library name
    _get_last_path_elem("${sap_location}" sap_name)

    if(NOT IS_ABSOLUTE "${sap_location}")
        set(sap_location "${CMAKE_CURRENT_SOURCE_DIR}/${sap_location}")
    endif()

    # Set the configuration and add define for current project
    if(NOT("${ARGV1}" STREQUAL ""))
        add_definitions(-D${ARGV1}=1)
        set(${ARGV1} yes CACHE STRING "")
        message("Adding Definition ${ARGV1}")
    endif()

    message(STATUS "Adding subdirectory ${sap_location} ${sap_name}")

    # Unset names as otherwise they (including ARGC etc) will be propagated down
    unset(ARGC)
    unset(ARGV1)
    unset(sap_len)

    add_subdirectory(${sap_location} ${sap_name})
    svs_prepend_to_target_lib(${sap_name})

endfunction()

# -----------------------------------------------------------------------------
function(_vs_set_working_directory)
    if(NOT MSVC)
        message(FATAL_ERROR "Trying to set working directory for unsupported environment")
    endif()
    set(userFileName "${CMAKE_BINARY_DIR}/${EXECUTABLE_NAME}.vcxproj.user")

    if(EXISTS "${userFileName}")
        return()
    endif()

    # Set a property for each configuration type
    foreach(curConfig ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${curConfig} cfgUp)
        set(tmp "${tmp}\n\t\t<LocalDebuggerWorkingDirectory Condition=\"'")
        set(tmp "${tmp}$(Configuration)|$(Platform)'=='${curConfig}|${SVS_IMPORTS_ARCH}'\">")
        set(tmp "${tmp}${CMAKE_RUNTIME_OUTPUT_DIRECTORY_${cfgUp}}</LocalDebuggerWorkingDirectory>")
    endforeach()

    set(USER_CONFIGURATIONS "${tmp}")

    set(template_name "${SVS_CMAKE_HELPER_DIR}/templates/windows/SvsUserFile.in")
    configure_file("${template_name}" "${userFileName}")

endfunction()

# -----------------------------------------------------------------------------
function(_create_copy_dependencies_target)
    if(TARGET COPY_DEPENDENCIES)
        return()
    endif()

    add_custom_target(COPY_DEPENDENCIES)
    if(MSVC)
        set_target_properties(COPY_DEPENDENCIES PROPERTIES FOLDER ${PREDEFINED_TARGETS_FOLDER})
    endif()

endfunction()

# -----------------------------------------------------------------------------
function(_ensure_install_directory_set)
   # If a custom install directory has not yet been set then set a default one.
   if(NOT SVS_INSTALL_DIR)
      message(STATUS "No install directory has been set. Using default value.")

      if (NOT SVS_TOP_LEVEL_PROJECT)
         message(FATAL_ERROR "No top level project value is set. You should make sure you're calling svs_init, and you may need to delete your build directory.")
      endif()
      
        if(WIN32)
            set(SVS_INSTALL_DIR "Sentient Vision/${SVS_TOP_LEVEL_PROJECT}" CACHE STRING "")
        else()
            set(SVS_INSTALL_DIR "/opt/Sentient Vision/${SVS_TOP_LEVEL_PROJECT}" CACHE STRING "")
        endif()
    endif()
endfunction()
