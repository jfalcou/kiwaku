##======================================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================

##======================================================================================================================
## Common compiler option
##======================================================================================================================
add_library(kiwaku_opts INTERFACE)

target_compile_features ( kiwaku_opts INTERFACE cxx_std_23 )

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
      target_compile_options( kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:/W3 /WX /bigobj /EHsc> )
  else()
      target_compile_options( kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:-Wshadow -Werror -Wall -Wextra -Wconversion -Wunused-variable -Wextra-semi>)
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options( kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:/W3 /WX /EHsc /bigobj /Zc:preprocessor>)
else()
    target_compile_options( kiwaku_opts INTERFACE $<$<COMPILE_LANGUAGE:CXX>:-Wshadow -Werror -Wall -Wextra -Wconversion -Wunused-variable -Wextra-semi>)
endif()

##======================================================================================================================
## Compiler options for Doc Tests
##======================================================================================================================
add_library(kiwaku_docs INTERFACE)

target_link_libraries(kiwaku_docs INTERFACE kiwaku_opts)
target_include_directories( kiwaku_docs INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )


##======================================================================================================================
## Compiler options for Unit Tests
##======================================================================================================================
add_library(kiwaku_test INTERFACE)

target_link_libraries(kiwaku_test INTERFACE kiwaku_opts kumi::kumi tts::tts)
target_include_directories( kiwaku_test INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )
