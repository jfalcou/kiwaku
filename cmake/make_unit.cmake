##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
include(add_target_parent)

##==================================================================================================
# Unit test Configuration
##==================================================================================================
add_library(kiwaku_test INTERFACE)

if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  target_compile_options( kiwaku_test INTERFACE /W3 /EHsc /std:c++20)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  target_compile_options( kiwaku_test INTERFACE -std=c++20 -Werror -Wall -Wextra -Wdocumentation -Wunused-variable)
else()
  target_compile_options( kiwaku_test INTERFACE -std=c++20 -Werror -Wall -Wextra -Wunused-variable)
endif()

target_include_directories( kiwaku_test INTERFACE
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
add_test( NAME ${test}
          WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/${location}"
          COMMAND $<TARGET_FILE:${test}>
        )
endfunction()

##==================================================================================================
## Process a list of source files to generate corresponding test target
##==================================================================================================
function(make_unit root)
  foreach(file ${ARGN})
    source_to_target( ${root} "exe" ${file} test)
    add_executable(${test} ${file})

    add_target_parent(${test})
    add_dependencies(unit ${test})

    setup_location( ${test} "unit")
    target_link_libraries(${test} PUBLIC kiwaku_test)
  endforeach()
endfunction()
