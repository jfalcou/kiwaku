cmake_minimum_required(VERSION 3.16)
project(__project_name__ LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)            # Enable c++17 standard

# For VSCode's clangd compatibility
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package( BLAS REQUIRED )
message( STATUS BLAS found: ${BLAS_LIBRARIES} )

##==================================================================================================
## Compiler options
##==================================================================================================
add_library(compiler_options INTERFACE)
add_library(unit_options INTERFACE)
add_library(bench_options INTERFACE)

target_compile_options( compiler_options INTERFACE -Werror -Wall -Wextra)
target_link_libraries ( compiler_options INTERFACE)

target_compile_options( unit_options INTERFACE -O0)
target_link_libraries ( unit_options INTERFACE compiler_options)

target_compile_options( bench_options INTERFACE -O3 -DNDEBUG)
target_link_libraries ( bench_options INTERFACE compiler_options)

##==================================================================================================
add_executable(std.out GEM_std.cpp ../GEM.cpp)

target_link_libraries(std.out PUBLIC bench_options ${BLAS_LIBRARIES}) 
