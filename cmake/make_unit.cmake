##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(add_target_parent)

##==================================================================================================
# Unit test Configuration
##==================================================================================================
add_library(unit_test_config INTERFACE)

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( unit_test_config INTERFACE /W3 /EHsc /std:c++latest)
else()
  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options( unit_test_config INTERFACE -std=c++20 -Werror -Wall -Wextra -Wno-gnu-string-literal-operator-template)
  else()
  target_compile_options( unit_test_config INTERFACE -std=c++20 -Werror -Wall -Wextra)
  endif()
endif()

target_include_directories( unit_test_config INTERFACE
                            ${PROJECT_SOURCE_DIR}/test
                            ${PROJECT_SOURCE_DIR}/include
                          )

##==================================================================================================
## Turn a filename to a dot-separated target name
##==================================================================================================
function(source_to_target root ext filename testname)
  string(REPLACE ".cpp" ".${ext}" base ${filename})
  string(REPLACE "/"    "." base ${base})
  string(REPLACE "\\"   "." base ${base})
  SET(${testname} "${root}.${base}" PARENT_SCOPE)
endfunction()

function(setup_location test location)
set_property( TARGET ${test}
              PROPERTY RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/${location}"
            )
if( CI_ENABLED )
add_test( NAME ${test}
          WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${location}"
          COMMAND $<TARGET_FILE:${test}> --no-color --pass
        )
else()
add_test( NAME ${test}
          WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${location}"
          COMMAND $<TARGET_FILE:${test}>
        )
endif()
endfunction()

##==================================================================================================
## Process a list of source files to generate corresponding test target
##==================================================================================================
function(make_unit root)
  foreach(file ${ARGN})
    source_to_target( ${root} "unit" ${file} test)
    add_executable(${test} ${file})

    add_target_parent(${test})
    add_dependencies(unit ${test})

    setup_location( ${test} "unit")
    target_link_libraries(${test} PUBLIC unit_test_config)

    target_include_directories( ${test}
                                PUBLIC
                                  $<INSTALL_INTERFACE:include>
                                  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                PRIVATE
                                  ${PROJECT_SOURCE_DIR}/test
                              )

  endforeach()
endfunction()
