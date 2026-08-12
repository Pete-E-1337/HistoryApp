#message("cmake -DBIN=${BIN} -DLIBS=${LIBS} -DCOPY_TARGET=${COPY_TARGET} -DSVS_INSTALL_DIR=${SVS_INSTALL_DIR} -DLDD=${LDD} -P ${CMAKE_CURRENT_LIST_DIR}/svs_cross_copylibs.cmake")
message(STATUS "Copying libraries to ${COPY_TARGET}...")

# Need to make sure we have access to LDD
if(LDD AND EXISTS "/usr/bin/${LDD}")
   message(STATUS "LDD script found...")
else()
   message(STATUS "LDD script not found! Copying now... sudo authentication is required...")
   execute_process(COMMAND sudo cp ${IMPORTS_DIR}/toolchains/xldd.sh /usr/bin/${LDD})
endif()

# binary details
get_filename_component(bindir ${BIN} DIRECTORY)
get_filename_component(binary_name ${BIN} NAME_WE)

#Run LDD
if(LDD STREQUAL "ldd") # In this case, we are using a native utility
   execute_process(COMMAND ${LDD} ${BIN} 
      OUTPUT_VARIABLE ldd_output)
else() # In this case, we are using a script which requires some additional arguments
   execute_process(COMMAND ${LDD} --root / ${BIN} 
   OUTPUT_VARIABLE ldd_output)      
endif()

#Split into lines for each lib
STRING(REGEX REPLACE ";" "\\\\;" ldd_output "${ldd_output}")
STRING(REGEX REPLACE "\n" ";" ldd_output "${ldd_output}")

# Add the target directory if it doesn't exist on remote
execute_process(COMMAND ssh ${COPY_TARGET} mkdir -p ${SVS_INSTALL_DIR})
execute_process(COMMAND ssh ${COPY_TARGET} mkdir -p ${SVS_INSTALL_DIR}/lib)

# Check if this is a copyable lib
foreach(libline ${ldd_output})
	STRING(REGEX MATCH "(\\/[^ ]*)+" libpath ${libline})
   STRING(REGEX MATCH "[^ ]*\\.so[^ ]*" libfile ${libline})
   if(libpath AND libfile)
      get_filename_component(libdir ${libpath} DIRECTORY)
      list(FIND LIBS ${libdir} found)
      if(NOT found EQUAL -1)
         message(STATUS "Copying ${libpath} to ${COPY_TARGET}:${SVS_INSTALL_DIR}/lib/")
         execute_process(COMMAND scp ${libpath} ${COPY_TARGET}:${SVS_INSTALL_DIR}/lib/)
      endif()
   endif()
endforeach(libline)

# Copy the contents of lib directory and copy all of these too
file(GLOB libs_in_build "${bindir}/../lib/*")
foreach(lib_in_build ${libs_in_build})
	get_filename_component(lib_in_build ${lib_in_build} ABSOLUTE)
	message(STATUS "Copying ${lib_in_build} to ${COPY_TARGET}:${SVS_INSTALL_DIR}/lib/")
	execute_process(COMMAND scp ${lib_in_build} ${COPY_TARGET}:${SVS_INSTALL_DIR}/lib/)
endforeach()
