cmake_minimum_required(VERSION 3.1)

project(EngineModule)

################################################################################
# Source files
################################################################################
file(GLOB COMMON_FILES
       ${PROJECT_SOURCE_DIR}/*.cpp
       ${PROJECT_SOURCE_DIR}/*.h)
	   
file(GLOB_RECURSE MODULE_FILES
       ${PROJECT_SOURCE_DIR}/Private/*.cpp
       ${PROJECT_SOURCE_DIR}/Public/*.h
	   )

file(GLOB_RECURSE THIRDPARTY_FILES
       ${PROJECT_SOURCE_DIR}/ThirdParty/*.h
       ${PROJECT_SOURCE_DIR}/ThirdParty/*.cpp
	   )

set(ALL_FILES
    ${COMMON_FILES}
    ${MODULE_FILES}
    ${THIRDPARTY_FILES}
	)
	
foreach(FILE ${ALL_FILES})
    # make rel path
	file(RELATIVE_PATH REL_PATH "${PROJECT_SOURCE_DIR}" "${FILE}")
	set(ALL_FILES_REL ${ALL_FILES_REL} ${REL_PATH})
endforeach()	

set_source_files_properties(${ALL_FILES_REL} PROPERTIES HEADER_FILE_ONLY FALSE)

foreach(FILE ${ALL_FILES_REL})
    # get the base path
    get_filename_component(BASE_PATH "${FILE}" PATH)

    # changes /'s to \\'s
    string(REPLACE "/" "\\" GROUP "${BASE_PATH}")

    source_group("${GROUP}" FILES "${FILE}")
endforeach()


################################################################################
# Include directory
################################################################################
include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Public
    ${PROJECT_SOURCE_DIR}/ThirdParty
    ${RUNTIME_MODULE_DIR}/Math/Public
	)


################################################################################
# Precompiled Headers
################################################################################
set(ARG_PCH_HEADER Precompiled.h)
set(ARG_PCH_SOURCE Precompiled.cpp)
foreach (FILE ${ALL_FILES_REL})
  if (FILE MATCHES ".*\\.(c|cpp)$")
	if (FILE STREQUAL ${ARG_PCH_SOURCE})
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yc\"${ARG_PCH_HEADER}\"")
	else()
	  set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yu\"${ARG_PCH_HEADER}\"")
	endif()
  endif()
endforeach(FILE)


################################################################################
# Library Output Setting
################################################################################
add_library(${PROJECT_NAME} STATIC ${ALL_FILES_REL})

set_target_properties(${PROJECT_NAME} PROPERTIES
    PREFIX ""
    OUTPUT_NAME ${PROJECT_NAME}
    #LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/Win64/Library
	ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Output/Win64/Library)
	

################################################################################
# Import Library
################################################################################
target_link_libraries(${PROJECT_NAME} MathModule)


