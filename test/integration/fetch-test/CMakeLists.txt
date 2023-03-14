##==================================================================================================
##  EVE - Expressive Vector Engine
##  Copyright : EVE Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.18)
project(kiwaku-fetch-test LANGUAGES CXX)
enable_testing()

message(STATUS "Testing FetchContent for commit ${kiwaku_git_head}")

# Enable FetchContent
include(FetchContent)

# Retrieve EVE from relative git directory
FetchContent_Declare(kiwaku
  GIT_REPOSITORY "${kiwaku_SOURCE_DIR}"
  GIT_TAG "${kiwaku_git_head}"
)

# make available
FetchContent_MakeAvailable(kiwaku)

add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
add_test(NAME test_kiwaku COMMAND test_kiwaku)
