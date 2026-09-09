##======================================================================================================================
##  KIWAKU - Shape Up Your Data
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.24)
project(kiwaku LANGUAGES CXX)

##======================================================================================================================
include(${PROJECT_SOURCE_DIR}/cmake/dependencies.cmake)

##======================================================================================================================
## Project setup via copacabana
##======================================================================================================================
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake ${COPACABANA_SOURCE_DIR}/copacabana/cmake)
include(${COPACABANA_SOURCE_DIR}/copacabana/cmake/copacabana.cmake)
copa_project_version(MAJOR 0 MINOR 1 PATCH 0 REPOSITORY https://github.com/jfalcou/kiwaku)

##======================================================================================================================
## Options, each remembered for the summary below
##======================================================================================================================
copa_add_option(KIWAKU_BUILD_TEST "Build tests for KIWAKU" ON LABEL "Unit tests")
copa_add_option(KIWAKU_BUILD_DOCUMENTATION "Build Doxygen for KIWAKU" OFF LABEL "Doxygen")
copa_add_option(KIWAKU_ENABLE_SANITIZERS "Build tests with AddressSanitizer + UndefinedBehaviorSanitizer" OFF LABEL
                "Sanitizers")
copa_add_option(KIWAKU_ENABLE_COVERAGE "Build tests with code coverage instrumentation" OFF LABEL "Coverage")

if(KIWAKU_BUILD_TEST)
  include(${PROJECT_SOURCE_DIR}/cmake/compiler.cmake)
endif()

##======================================================================================================================
## Summary Display, which KIWAKU_QUIET silences on its own; QUIET_OPTION is for the calls below
##======================================================================================================================
copa_show_options()

if(NOT KIWAKU_QUIET)
  set(QUIET_OPTION "")
else()
  set(QUIET_OPTION "QUIET")
endif()

##======================================================================================================================
## Pre-commit hooks setup targets
##======================================================================================================================
copa_setup_precommit_hooks(${QUIET_OPTION})

##======================================================================================================================
## Install Process setup
##======================================================================================================================
copa_setup_install(
  LIBRARY kiwaku
  FEATURES cxx_std_20
  DOC ${PROJECT_SOURCE_DIR}/LICENSE.md
  INCLUDE ${PROJECT_SOURCE_DIR}/include/kiwaku)

##======================================================================================================================
## Setup doxygen
##======================================================================================================================
if(KIWAKU_BUILD_DOCUMENTATION)
  copa_setup_doxygen(
    ${QUIET_OPTION}
    TARGET kiwaku-doxygen
    DESTINATION "${PROJECT_BINARY_DIR}/doc"
    GODBOLT_LIBRARIES kiwaku tts
    COLOR_HUE 293
    COLOR_SATURATION 40)
endif()

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(KIWAKU_BUILD_TEST)
  if(KIWAKU_ENABLE_SANITIZERS)
    copa_setup_sanitizers(kiwaku_tests ENABLE_ASAN ENABLE_UBSAN)
  endif()

  enable_testing()
  add_subdirectory(test)

  # After the test targets exist, so the coverage targets can depend on them
  if(KIWAKU_ENABLE_COVERAGE)
    copa_setup_coverage(kiwaku_tests)
  endif()
endif()
