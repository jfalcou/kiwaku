##======================================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.22)
project(kiwaku LANGUAGES CXX)

##======================================================================================================================
option( KIWAKU_BUILD_TEST           "Build tests   for Kiwaku"            ON  )
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
                    INCLUDE ${PROJECT_SOURCE_DIR}/include/kwk
                  )

##======================================================================================================================
## Setup doxygen
##======================================================================================================================
if(KIWAKU_BUILD_DOCUMENTATION)
  copa_setup_doxygen(${QUIET_OPTION} TARGET kiwaku-doxygen DESTINATION "${PROJECT_BINARY_DIR}/doc")
endif()


option(KIWAKU_BUILD_BENCH       "Build benchmarks." ON)
option(KIWAKU_BENCH_SYCL        "Use SYCL in benchmark tests. You mush use a SYCL-capabable compiler." OFF)
option(KIWAKU_BENCH_MTHREAD     "Use multi-thread in benchmark tests." OFF)
option(KIWAKU_BENCH_EVE         "Use EVE SIMD in benchmark tests." ON)
option(KIWAKU_ONLY_BENCH_GPU    "Ony bench GPU execution and EVE SIMD, to save time." OFF)

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(KIWAKU_BUILD_TEST)
  enable_testing()
  add_custom_target(kiwaku-unit)
  add_subdirectory(test)
endif()

if(KIWAKU_BUILD_BENCH)
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BUILD_BENCH)
endif()

if(KIWAKU_BENCH_EVE)
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_EVE)
endif()

if(KIWAKU_ONLY_BENCH_GPU  )
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_ONLY_BENCH_GPU  )
endif()

if(KIWAKU_BENCH_SYCL)
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_SYCL)
endif()

if(KIWAKU_BENCH_MTHREAD)
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_MTHREAD)
endif()

# add_executable(find_1d test/unit/bench/find_1d.cpp)

# target_link_libraries(find_1d PRIVATE kiwaku::kiwaku raberu::raberu kumi::kumi)

# add_test(NAME find_1d_bench COMMAND find_1d)

# target_link_libraries(find_1d PRIVATE kiwaku::kiwaku)
# target_link_libraries(find_1d PRIVATE raberu)

# if(KWK_ENABLE_CUSTOM_FEATURE)
  #   target_compile_definitions(kiwaku-unit PRIVATE KWK_ENABLE_CUSTOM_FEATURE)
  # endif()
