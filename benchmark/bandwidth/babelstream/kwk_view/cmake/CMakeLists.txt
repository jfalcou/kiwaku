cmake_minimum_required(VERSION 3.16)

project(__project_name__ LANGUAGES CXX)

# For VSCode's clangd compatibility
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(dependencies.cmake)

##==================================================================================================
## Compiler options
##==================================================================================================
add_library(kwk_compiler_options INTERFACE)

# Benchmark à modifier
set(CMAKE_CXX_FLAGS "-O3")

# target_compile_features ( kwk_compiler_options INTERFACE  cxx_std_20 )
# if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
#   target_compile_options( kwk_compiler_options INTERFACE /W3 /EHsc)
# else()
#   target_compile_options( kwk_compiler_options INTERFACE -Werror -Wall -Wextra)
# endif()

target_link_libraries(kwk_compiler_options INTERFACE kiwaku::kiwaku)
##==================================================================================================



message(STATUS "Testing = ${kiwaku_SOURCE_DIR}")

add_executable(kwk.out ../babelstream_kwk.cpp)

target_link_libraries(kwk.out PUBLIC kwk_compiler_options) #ma_target_avec_kiwaku
