##======================================================================================================================
##  KIWAKU - C++20 collection of high performance data containers and related tools
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
cmake_minimum_required(VERSION 3.22)
project(kiwaku-install-test LANGUAGES CXX)
enable_testing()

find_package(kiwaku CONFIG REQUIRED)
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
add_test(NAME test_kiwaku COMMAND test_kiwaku)
