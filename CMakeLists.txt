##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.18)
enable_testing()

## =================================================================================================
## Setup project
## =================================================================================================
project(kiwaku LANGUAGES CXX)

set(KIWAKU_MAJOR_VERSION 0)
set(KIWAKU_MINOR_VERSION 1)
set(KIWAKU_PATCH_VERSION 0)
set(KIWAKU_VERSION ${KIWAKU_MAJOR_VERSION}.${KIWAKU_MINOR_VERSION}.${KIWAKU_PATCH_VERSION})

set(PROJECT_VERSION   ${KIWAKU_VERSION})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake )

##==================================================================================================
## Prevent in-source build
##==================================================================================================
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message( FATAL_ERROR "[kiwaku] Building in-source, not recommended! Build in a separate directory." )
endif()

## =================================================================================================
## Exporting target for external use
## =================================================================================================
include(kiwaku-install)

##==================================================================================================
## Options
##==================================================================================================
option( KIWAKU_BUILD_TEST         "Build tests for Kiwaku" ON               )
option( KIWAKU_BUILD_INTEGRATION  "Build integration tests for Kiwaku" OFF  )
option( KIWAKU_BUILD_DOCS         "Build docs for Kiwaku"  OFF              )

##==================================================================================================
## Setup Testing
##==================================================================================================
if( KIWAKU_BUILD_TEST )
  enable_testing()
  include(CTest)
  add_custom_target(unit)
  add_subdirectory(test)
endif()

if( KIWAKU_BUILD_INTEGRATION )
  add_subdirectory("${PROJECT_SOURCE_DIR}/test/integration")
endif()

##==================================================================================================
## Add Doxygen building target
##==================================================================================================
if(KIWAKU_BUILD_DOCS)
  find_package(Doxygen QUIET)

  if (DOXYGEN_FOUND)
    message( STATUS "[kiwaku] Doxygen available")
  else (DOXYGEN_FOUND)
    message( STATUS "[kiwaku] Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)

  if (DOXYGEN_FOUND)
  set(DOXYGEN_CONFIG ${PROJECT_SOURCE_DIR}/doc/Doxyfile)
  add_custom_target ( doxygen
                      COMMAND KWK_DOXYGEN_OUPUT=../docs ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
                      COMMENT "[kiwaku] Generating API documentation with Doxygen"
                      VERBATIM
                    )

  add_custom_target ( doxygen-local
                      COMMAND KWK_DOXYGEN_OUPUT=${PROJECT_BINARY_DIR}/docs ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
                      COMMENT "[kiwaku] Generating API documentation with Doxygen - Local version"
                      VERBATIM
                    )
  endif (DOXYGEN_FOUND)
endif()
