cmake_minimum_required(VERSION 3.16)
project(__project_name__ LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)            # Enable c++17 standard

# For VSCode's clangd compatibility
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(dependencies.cmake)

##==================================================================================================
## Compiler options
##==================================================================================================
add_library(kwk_compiler_options INTERFACE)
add_library(kwk_unit_options INTERFACE)
add_library(kwk_bench_options INTERFACE)

target_compile_options( kwk_compiler_options INTERFACE -Werror -Wall -Wextra)
target_link_libraries ( kwk_compiler_options INTERFACE kiwaku::kiwaku)

target_compile_options( kwk_unit_options INTERFACE -O0)
target_link_libraries ( kwk_unit_options INTERFACE kwk_compiler_options)

target_compile_options( kwk_bench_options INTERFACE -O3 -DNDEBUG)
target_link_libraries ( kwk_bench_options INTERFACE kwk_compiler_options)


##==================================================================================================

message(STATUS "Testing = ${kiwaku_SOURCE_DIR}")

add_executable(test.out ../test.cpp )

target_link_libraries(test.out PUBLIC kwk_compiler_options) #ma_target_avec_kiwaku
