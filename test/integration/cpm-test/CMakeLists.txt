##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.19)
project(kwk-cpm-test LANGUAGES CXX)
enable_testing()

# Setup CPM
include(cpm.cmake)

# Add dependencies
CPMAddPackage ( NAME kiwaku
                GIT_REPOSITORY  "${kiwaku_SOURCE_DIR}"
                GIT_TAG         "${kiwaku_git_head}"
                OPTIONS         "KIWAKU_BUILD_TEST OFF"
                                "KIWAKU_BUILD_DOCS OFF"
              )

message(STATUS "Testing CPM for commit ${kiwaku_git_head} in ${kiwaku_SOURCE_DIR}")

# Use EVE
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
add_test(NAME test_kiwaku COMMAND test_kiwaku)
