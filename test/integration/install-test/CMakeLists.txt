##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
cmake_minimum_required(VERSION 3.19)
project(kwk-install-test LANGUAGES CXX)

## Points to the installation folder for KIWAKU
set(CMAKE_PREFIX_PATH "/usr/local/lib")

find_package(kiwaku)
add_executable(test_kiwaku ../main.cpp)
target_link_libraries(test_kiwaku PUBLIC ${KIWAKU_LIBRARIES})
