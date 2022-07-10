##==================================================================================================
##  EVE - Expressive Vector Engine
##  Copyright : EVE Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
cmake_minimum_required(VERSION 3.19)
project(kiwaku-install-test LANGUAGES CXX)

message(STATUS "Testing FetchContent for branch ${KIWAKU_SHA1}")

# Setup CPM
include(cpm.cmake)

# Add dependencies
CPMAddPackage ( NAME kiwaku
                GITHUB_REPOSITORY jfalcou/kiwaku
                GIT_TAG ${KIWAKU_SHA1}
              )

# Use kiwaku
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC  ${KIWAKU_LIBRARIES})
