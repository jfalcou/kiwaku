##======================================================================================================================
##  KIWAKU - C++20 collection of high performance data containers and related tools
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Compiler options for Tests
##======================================================================================================================
add_library(kiwaku_tests INTERFACE)

target_compile_features(kiwaku_tests INTERFACE cxx_std_20)

## Held in a variable so the calls below stay on one line: a wrapped argument list aligns its continuation on the
## column the list opens at, which moves with the length of the target's name.
set(KIWAKU_WARNINGS -Werror -Wall -Wextra -Wunused-variable -Wshadow)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options(kiwaku_tests INTERFACE $<$<COMPILE_LANGUAGE:CXX>:/W3 /EHsc>)
  else()
    target_compile_options(kiwaku_tests INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${KIWAKU_WARNINGS} -Wdocumentation>)
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(kiwaku_tests INTERFACE $<$<COMPILE_LANGUAGE:CXX>:/W3 /EHsc /Zc:preprocessor>)
else()
  target_compile_options(kiwaku_tests INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${KIWAKU_WARNINGS}>)
endif()

target_include_directories(kiwaku_tests INTERFACE ${PROJECT_SOURCE_DIR}/test ${PROJECT_SOURCE_DIR}/include)
