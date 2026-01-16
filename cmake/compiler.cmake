##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================

##======================================================================================================================
## Compiler options for Doc Tests
##======================================================================================================================
add_library(kiwaku_docs INTERFACE)

target_compile_features ( kiwaku_docs INTERFACE  cxx_std_20 )

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if(CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
    target_compile_options( kiwaku_docs INTERFACE /W3 /bigobj  /EHsc )
  else()
    target_compile_options( kiwaku_docs INTERFACE -Wshadow -Werror -Wall -Wextra -Wconversion -Wunused-variable)
  endif()
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( kiwaku_docs INTERFACE /W3 /EHsc /bigobj  /Zc:preprocessor)
else()
  target_compile_options( kiwaku_docs INTERFACE -Wshadow -Werror -Wall -Wextra -Wconversion -Wunused-variable)
endif()

target_include_directories( kiwaku_docs INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )

target_link_libraries(kiwaku_docs INTERFACE kumi::kumi raberu::raberu eve::eve)

##======================================================================================================================
## Compiler options for Unit Tests
##======================================================================================================================
add_library(kiwaku_test INTERFACE)
target_include_directories( kiwaku_test INTERFACE
                            ${PROJECT_SOURCE_DIR}/test/unit
                          )
target_link_libraries(kiwaku_test INTERFACE kiwaku_docs tts::tts)
