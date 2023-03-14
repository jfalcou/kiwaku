##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.19)
project(kwk-install-test LANGUAGES CXX)

## Points to the installation folder for KIWAKU

find_package(kiwaku CONFIG REQUIRED)
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC kiwaku::kiwaku)
add_test(NAME test_kiwaku COMMAND test_kiwaku)
