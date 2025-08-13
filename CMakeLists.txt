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

# TODO à modifier dans le code:
# KIWAKU_BENCH_SYCL
# KIWAKU_BENCH_MTHREAD
# KIWAKU_ONLY_BENCH_GPU  -> KIWAKU_BENCH_SYCL_GPU_ONLY
# KIWAKU_BUILD_BENCH -> KIWAKU_BENCH_BUILD

option(KIWAKU_BENCH_BUILD         "Build benchmarks." ON)
option(KIWAKU_BENCH_ASSERT        "Use TTS to check benchmark results." ON)
option(KIWAKU_BENCH_FAST_INIT     "Initialize arrays to the same value, and do not use random trigonometric initialization, to save time." OFF)
option(KIWAKU_BENCH_STD_SEQ       "Benchmark std::seq execution policy." ON)
option(KIWAKU_BENCH_STD_UNSEQ     "Benchmark std::unseq execution policy." ON)
option(KIWAKU_BENCH_STD_PAR       "Benchmark std::par execution policy." OFF) # KIWAKU_BENCH_MTHREAD
option(KIWAKU_BENCH_STD_PAR_UNSEQ "Benchmark std::par_unseq execution policy." OFF)
option(KIWAKU_BENCH_HAND          "Benchmark hand-written loop (no Kiwaku)." ON)
option(KIWAKU_BENCH_KWK_SEQ       "Benchmark Kiwaku's CPU sequential execution context." ON)
option(KIWAKU_BENCH_KWK_SIMD      "Benchmark Kiwaku's EVE SIMD execution context." ON)
option(KIWAKU_BENCH_KWK_SYCL_CPU  "Benchmark CPU parallel execution with SYCL. You must use a SYCL-capable compiler." OFF)
option(KIWAKU_BENCH_KWK_SYCL_GPU  "Benchmark GPU offloading with SYCL. You must use a SYCL-capable compiler." OFF)

# Anciennement DESPAIR mis directement dans le code par manque de temps
# option(KIWAKU_BENCH_SYCL_GPU_ONLY "Ony benchmark GPU execution, to save time." OFF) #  and EVE SIMD

set(KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz "3.6" CACHE STRING "The average clock speed of a single CPU core under heavy load.")
set(KIWAKU_BENCH_MEMORY_DATA_SIZE_MB "4096" CACHE STRING "The size (in megabytes) of the array used for memory-bound benchmarks.")
set(KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB "1024" CACHE STRING "The size (in megabytes) of the array used for compute-bound benchmarks.")
set(KIWAKU_BENCH_L2_SIZE_KB "256" CACHE STRING "The size (in kilobytes) of the L2 cache of this system. Bor L2 benchmarks, this size is divided by two.")

##======================================================================================================================
## Tests setup
##======================================================================================================================
if(KIWAKU_BUILD_TEST)
  enable_testing()
  add_custom_target(kiwaku-unit)
  add_subdirectory(test)

  if(KIWAKU_BENCH_BUILD)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_BUILD)
  endif()
  if(KIWAKU_BENCH_ASSERT)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_ASSERT)
  endif()
  if(KIWAKU_BENCH_FAST_INIT)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_FAST_INIT)
  endif()
  if(KIWAKU_BENCH_HAND)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_HAND)
  endif()
  if(KIWAKU_BENCH_KWK_SEQ)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_KWK_SEQ)
  endif()
  if(KIWAKU_BENCH_KWK_SIMD)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_KWK_SIMD)
  endif()
  if(KIWAKU_BENCH_STD_SEQ)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_STD_SEQ)
  endif()
  if(KIWAKU_BENCH_STD_UNSEQ)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_STD_UNSEQ)
  endif()
  if(KIWAKU_BENCH_STD_PAR)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_STD_PAR)
  endif()
  if(KIWAKU_BENCH_STD_PAR_UNSEQ)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_STD_PAR_UNSEQ)
  endif()
  if(KIWAKU_BENCH_KWK_SYCL_CPU)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_KWK_SYCL_CPU)
  endif()
  if(KIWAKU_BENCH_KWK_SYCL_GPU)
    target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_KWK_SYCL_GPU)
  endif()

  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz=${KIWAKU_BENCH_CPU_CLOCK_SPEED_GHz})
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_MEMORY_DATA_SIZE_MB=${KIWAKU_BENCH_MEMORY_DATA_SIZE_MB})
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB=${KIWAKU_BENCH_COMPUTE_DATA_SIZE_MB})
  target_compile_definitions(kiwaku_test INTERFACE KIWAKU_BENCH_L2_SIZE_KB=${KIWAKU_BENCH_L2_SIZE_KB})
endif()
# add_executable(find_1d test/unit/bench/find_1d.cpp)

# target_link_libraries(find_1d PRIVATE kiwaku::kiwaku raberu::raberu kumi::kumi)

# add_test(NAME find_1d_bench COMMAND find_1d)

# target_link_libraries(find_1d PRIVATE kiwaku::kiwaku)
# target_link_libraries(find_1d PRIVATE raberu)

# if(KWK_ENABLE_CUSTOM_FEATURE)
  #   target_compile_definitions(kiwaku-unit PRIVATE KWK_ENABLE_CUSTOM_FEATURE)
  # endif()
