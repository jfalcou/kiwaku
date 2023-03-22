cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FlAGS "-O3")

project(

    CMakeNanobench

    VERSION 1.0

    LANGUAGES CXX)


include(FetchContent)


FetchContent_Declare(

    nanobench

    GIT_REPOSITORY https://github.com/martinus/nanobench.git

    GIT_TAG v4.1.0

    GIT_SHALLOW TRUE)


FetchContent_MakeAvailable(nanobench)


add_executable(standalone babelstream_standalone.cpp)

target_link_libraries(standalone PRIVATE nanobench)
