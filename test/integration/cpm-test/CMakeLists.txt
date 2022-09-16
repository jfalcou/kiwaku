##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.19)
project(kiwaku-install-test LANGUAGES CXX)

message(STATUS "Testing CPM for branch ${KIWAKU_SHA1}")

# Setup CPM
include(cpm.cmake)

# Add dependencies
CPMAddPackage ( NAME kiwaku
                GITHUB_REPOSITORY jfalcou/kiwaku
                GIT_TAG ${KIWAKU_SHA1}
              )

# Use kiwaku
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
