cmake_minimum_required(VERSION 3.16)
project(__project_name__ LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

# For VSCode's clangd compatibility
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

##==================================================================================================
## Compiler options
##==================================================================================================
add_library(std_compiler_options INTERFACE)
add_library(std_unit_options INTERFACE)
add_library(std_bench_options INTERFACE)

target_compile_options( std_compiler_options INTERFACE -Werror -Wall -Wextra)

target_compile_options( std_unit_options INTERFACE -O0)
target_link_libraries ( std_unit_options INTERFACE std_compiler_options)

target_compile_options( std_bench_options INTERFACE -O3 -DNDEBUG)
target_link_libraries ( std_bench_options INTERFACE std_compiler_options)

##==================================================================================================

add_executable(bsearch.out ../bsearch_std.cpp ../../bsearch.cpp)

target_link_libraries(bsearch.out PUBLIC std_compiler_options) 
