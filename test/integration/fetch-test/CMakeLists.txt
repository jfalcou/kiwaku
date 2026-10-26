##======================================================================================================================
##  KIWAKU - Shape Up Your Data
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.22)
project(kiwaku-fetch-test LANGUAGES CXX)
enable_testing()

message(STATUS "Testing FetchContent for branch ${GIT_BRANCH}")

# Enable FetchContent
include(FetchContent)

# Retrieve KIWAKU from relative git directory
FetchContent_Declare(kiwaku GIT_REPOSITORY "https://github.com/jfalcou/kiwaku.git" GIT_TAG ${GIT_BRANCH})

## FetchContent has no per-package OPTIONS: what kiwaku reads is pinned here.
set(KIWAKU_BUILD_TEST OFF CACHE BOOL "" FORCE)

# make available
FetchContent_MakeAvailable(kiwaku)

add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
add_test(NAME test_kiwaku COMMAND test_kiwaku)
