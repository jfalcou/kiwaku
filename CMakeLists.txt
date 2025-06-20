##======================================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.22)
project(kiwaku LANGUAGES CXX)

##======================================================================================================================
option( KIWAKU_BUILD_TEST           "Build tests   for Kiwaku"            ON  )
option( KIWAKU_BUILD_INTEGRATION    "Build integration tests for Kiwaku"  OFF )
option( KIWAKU_BUILD_DOCUMENTATION  "Build Doxygen for Kiwaku"            OFF )

##======================================================================================================================
include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

if(KIWAKU_BUILD_TEST)
  include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)
endif()

##======================================================================================================================
## Project setup via copacabana
##======================================================================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake ${COPACABANA_SOURCE_DIR}/copacabana/cmake)
include(${COPACABANA_SOURCE_DIR}/copacabana/cmake/copacabana.cmake)
copa_project_version(MAJOR 0 MINOR 1 PATCH 0)

##======================================================================================================================
## Summary Display
##======================================================================================================================
if(NOT KIWAKU_QUIET)
  if(CMAKE_BUILD_TYPE)
    message(STATUS "[${PROJECT_NAME}] - Building in ${CMAKE_BUILD_TYPE} mode")
  endif()
  message(STATUS "[${PROJECT_NAME}] - Unit tests : ${KIWAKU_BUILD_TEST} (via KIWAKU_BUILD_TEST)")
  message(STATUS "[${PROJECT_NAME}] - Doxygen    : ${KIWAKU_BUILD_DOCUMENTATION} (via KIWAKU_BUILD_DOCUMENTATION)")
  set(QUIET_OPTION "")
else()
  set(QUIET_OPTION "QUIET")
endif()

##======================================================================================================================
## Install Process setup
##======================================================================================================================
copa_setup_install( LIBRARY kiwaku
                    FEATURES cxx_std_20
                    DOC     ${PROJECT_SOURCE_DIR}/LICENSE.md
                    INCLUDE ${PROJECT_SOURCE_DIR}/include/kiwaku
                  )

##======================================================================================================================
## Setup doxygen
##======================================================================================================================
if(KIWAKU_BUILD_DOCUMENTATION)
  copa_setup_doxygen(${QUIET_OPTION} TARGET kiwaku-doxygen DESTINATION "${PROJECT_BINARY_DIR}/doc")
endif()

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(KIWAKU_BUILD_TEST)
  enable_testing()
  add_custom_target(kiwaku-unit)
  add_subdirectory(test)
endif()

