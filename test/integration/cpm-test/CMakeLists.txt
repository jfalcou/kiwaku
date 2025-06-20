##======================================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.18)
project(kiwaku-cpm-test LANGUAGES CXX)
enable_testing()

message(STATUS "Testing CPM for branch ${GIT_BRANCH}")

# Setup CPM
include(cpm.cmake)

# Add dependencies
CPMAddPackage ( NAME kiwaku
                GIT_REPOSITORY "https://github.com/jfalcou/kiwaku.git"
                GIT_TAG "${GIT_BRANCH}"
                OPTIONS "KIWAKU_BUILD_TEST OFF"
              )

# Use kiwaku
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku kumi::kumi raberu::raberu)
add_test(NAME test_kiwaku COMMAND test_kiwaku)
