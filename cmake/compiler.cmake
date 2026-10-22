##======================================================================================================================
##  KIWAKU - Shape Up Your Data
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Compiler options for Tests
##======================================================================================================================
add_library(kiwaku_opts INTERFACE)

target_compile_features(kiwaku_opts INTERFACE cxx_std_23)

## Held in a variable so the calls below stay on one line: a wrapped argument list aligns its continuation on the
## column the list opens at, which moves with the length of the target's name.
set(KIWAKU_LINUX_WARNINGS -Werror -Wall -Wextra -Wunused-variable -Wshadow -Wextra-semi)
set(KIWAKU_WINDOWS_WARNING /W3 /Wx /EHsc)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      target_compile_options(kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${KIWAKU_WINDOWS_WARNING}>)
  else()
      target_compile_options(kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${KIWAKU_LINUX_WARNINGS} -Wdocumentation>)
  endif()
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${KIWAKU_WINDOWS_WARNING} /Zc:preprocessor>)
else()
    target_compile_options(kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:${KIWAKU_LINUX_WARNINGS}>)
endif()

target_include_directories(kiwaku_opts INTERFACE ${PROJECT_SOURCE_DIR}/test ${PROJECT_SOURCE_DIR}/include)

##======================================================================================================================
## Compiler options for Doc Tests
##======================================================================================================================
add_library(kiwaku_docs INTERFACE)

target_link_libraries(kiwaku_docs INTERFACE kiwaku_opts kumi::kumi)
target_include_directories( kiwaku_docs INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )

##======================================================================================================================
## Compiler options for Unit Tests of include/
##======================================================================================================================
add_library(kiwaku_test INTERFACE)

target_link_libraries(kiwaku_test INTERFACE kiwaku_opts kumi::kumi tts::tts)
target_include_directories( kiwaku_test INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )
