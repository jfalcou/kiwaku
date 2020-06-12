##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
include(add_target_parent)
include(download)

##==================================================================================================
# Unit test Configuration
##==================================================================================================
add_library(unit_test_config INTERFACE)
target_compile_features ( unit_test_config INTERFACE  cxx_std_20 )
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options( unit_test_config INTERFACE /W3 /EHsc)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_compile_options( unit_test_config INTERFACE -Wall -Werror)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  target_compile_options( unit_test_config INTERFACE -Wall -Werror)
endif()

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
    target_link_libraries(${test} PUBLIC tts PUBLIC unit_test_config)

    target_include_directories( ${test}
                                PUBLIC
                                  $<INSTALL_INTERFACE:include>
                                  $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                PRIVATE
                                  ${tts_SOURCE_DIR}/include
                                  ${PROJECT_SOURCE_DIR}/test
                              )

  endforeach()
endfunction()

##==================================================================================================
## Create a test that succeed if its compilation fails
##==================================================================================================
function(check_failure root)
  foreach(file ${ARGN})
    source_to_test( ${root} ${file} test)

    if( MSVC )
      set( options /std:c++latest /W3 /EHsc)
    else()
      set( options -std=c++17 -Wall -Wno-missing-braces )
    endif()

    set( test_lib "${test}_lib")
    add_library( ${test_lib} OBJECT EXCLUDE_FROM_ALL ${file})
    target_link_libraries( ${test_lib} PUBLIC unit_test_config)

    add_test( NAME ${test}
              COMMAND ${CMAKE_COMMAND} --build . --target ${test_lib} --config $<CONFIGURATION>
              WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/unit"
            )

    set_target_properties ( ${test_lib} PROPERTIES
                            EXCLUDE_FROM_DEFAULT_BUILD TRUE
                            EXCLUDE_FROM_ALL TRUE
                            ${MAKE_UNIT_TARGET_PROPERTIES}
                          )

    target_include_directories( ${test_lib}
                                PRIVATE
                                  ${tts_SOURCE_DIR}/include
                                  ${PROJECT_SOURCE_DIR}/test
                                  ${PROJECT_SOURCE_DIR}/include
                              )

    set_tests_properties ( ${test} PROPERTIES WILL_FAIL TRUE)

    add_dependencies(unit ${test})
    add_target_parent(${test})

  endforeach()
endfunction()

##==================================================================================================
## Setup TTS
##==================================================================================================
set(TTS_BUILD_TEST    OFF CACHE INTERNAL "OFF")
set(TTS_BUILD_DOC     OFF CACHE INTERNAL "OFF")
set(TTS_IS_DEPENDENT  ON  CACHE INTERNAL "ON")

download_project( PROJ                tts
                  GIT_REPOSITORY      https://github.com/jfalcou/tts.git
                  GIT_TAG             master
                  "UPDATE_DISCONNECTED 1"
                  QUIET
                )

add_subdirectory(${tts_SOURCE_DIR} ${tts_BINARY_DIR})
