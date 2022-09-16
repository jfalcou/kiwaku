##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.19)
project(test_kiwaku-install-test LANGUAGES CXX)

message(STATUS "Testing FetchContent for branch ${KIWAKU_SHA1}")

# Enable FetchContent
include(FetchContent)

# Retrieve KIWAKU
FetchContent_Declare(kiwaku
  GIT_REPOSITORY https://github.com/jfalcou/kiwaku.git
  GIT_TAG        ${KIWAKU_SHA1}
  )

FetchContent_MakeAvailable(kiwaku)

# make available
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
