##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
cmake_minimum_required(VERSION 3.5)

##==================================================================================================
## Setup project
##==================================================================================================
project(KIWAKU VERSION 0.1 LANGUAGES CXX)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

##==================================================================================================
## Prevent in-source build
##==================================================================================================
if(${PROJECT_SOURCE_DIR} STREQUAL ${PROJECT_BINARY_DIR})
  message( FATAL_ERROR "[kiwaku] Building in-source, not recommended! Build in a separate directory." )
endif()

##==================================================================================================
## Options
##==================================================================================================
option( KIWAKU_BUILD_TEST    "Build tests for kiwaku"     ON  )
option( KIWAKU_IS_DEPENDENT  "Dependent mode for kiwaku"  OFF )

##==================================================================================================
## Setup Install target only if not used as dependent
##==================================================================================================
# UNCOMMENT WHEN SOMETHING CAN BE INSTALLED
# if( NOT KIWAKU_IS_DEPENDENT )
#   include(install)
# else()
#   message(STATUS "[kiwaku] KIWAKU is used as a in-project dependency - No install targets available")
# endif()

##==================================================================================================
## Setup Testing
##==================================================================================================
if( KIWAKU_BUILD_TEST )
  enable_testing()
  include(CTest)
  add_custom_target(tests)
  add_custom_target(unit)

  add_dependencies(tests unit)
  add_subdirectory(test)
endif()

##==================================================================================================
## Add Doxygen building target
##==================================================================================================
find_package(Doxygen QUIET)

if (DOXYGEN_FOUND)
  message( STATUS "[kiwaku] Doxygen available")
else (DOXYGEN_FOUND)
  message( STATUS "[kiwaku] Doxygen need to be installed to generate the doxygen documentation")
endif (DOXYGEN_FOUND)

if (DOXYGEN_FOUND)
  set(DOXYGEN_CONFIG ${PROJECT_SOURCE_DIR}/doc/Doxyfile)
  add_custom_target ( doxygen
                      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
                      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/doc
                      COMMENT "[kiwaku] Generating API documentation with Doxygen"
                      VERBATIM
                    )
endif (DOXYGEN_FOUND)
