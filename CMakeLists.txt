##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
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
option( KIWAKU_BUILD_DOC     "Build doc for kiwaku"       ON  )
option( KIWAKU_IS_DEPENDENT  "Dependent mode for kiwaku"  OFF )

##==================================================================================================
## Setup Documentation
##==================================================================================================
if( KIWAKU_BUILD_DOC )
  add_subdirectory(doc)
endif()

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
