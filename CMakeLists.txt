################################################################################
#                          project attribution setting                         #
#[Project / Moudle_type]
#   BINARY      :    execute binary                                            #
#   STATIC      :    static library                                            #
#   SHARED      :    shared library                                            #
################################################################################
cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 11)

SET(PRIVATE_MODULE_NAME  pcre_demo)
SET(PRIVATE_MODULE_TYPE  "BINARY")
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/lib")

cmake_minimum_required(VERSION 2.8)

################################################################################
#                   dependent linked third library setting                     #
#CMAKE_CURRENT_SOURCE_DIR   CMakeList.txt's directory                          #
#CMAKE_CURRENT_BINARY_DIR   CMake target directory                             #
################################################################################

SET(PRIVATE_STATIC_LIBRARY
)

SET(PRIVATE_DYNAMIC_LIBRARY
    pcre
)
LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/lib)

# add library, directly add it
LINK_LIBRARIES(${PRIVATE_DYNAMIC_LIBRARY})
# add library, but it's need depend project
#TARGET_LINK_LIBRARIES(${PRIVATE_MODULE_NAME} ${PRIVATE_STATIC_LIBRARY})

################################################################################
#                        complier options setting                              #
################################################################################
#SET(CMAKE_CXX_FLAGS [Cpp compiler options])
SET(CMAKE_C_FLAGS   "-g")

################################################################################
#                       header file configurer setting                         #
#  Refs:                                                                       #
#  PROJECT_SOURCE_DIR        : top level source directory                      #
#  CMAKE_CURRENT_BINARY_DIR  : target complie directory                        #
################################################################################
SET(PRIVATE_MODULE_INC_PATH

)

INCLUDE_DIRECTORIES(

)

################################################################################
#                       source file configurer setting                         #
#  Refs:                                                                       #
#  PROJECT_SOURCE_DIR      : top level source directory of current project     #
################################################################################
SET(PRIVATE_STATIC_SOURCE
    main.c
)

################################################################################
#                           generate project target                            #
################################################################################

project(${PRIVATE_MODULE_NAME})

IF (${PRIVATE_MODULE_TYPE} MATCHES "BINARY")
    ADD_EXECUTABLE(${PRIVATE_MODULE_NAME} ${PRIVATE_STATIC_SOURCE})
ELSEIF (${PRIVATE_MODULE_TYPE} MATCHES "STATIC")
    ADD_LIBRARY(${PRIVATE_MODULE_NAME} STATIC ${PRIVATE_STATIC_SOURCE})
ELSEIF (${PRIVATE_MODULE_TYPE} MATCHES "SHARED")
    ADD_LIBRARY(${PRIVATE_MODULE_NAME} SHARED ${PRIVATE_STATIC_SOURCE})
ELSEIF ()
    MESSAGE (FATAL_ERROR "Sorry target type do not support")
ENDIF ()
