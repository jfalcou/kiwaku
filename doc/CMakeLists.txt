##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================

##==================================================================================================
## Setup the documentation
##==================================================================================================
find_package(Sphinx)

if(SPHINX_FOUND)
find_package(Doxygen)

# Find all the headers
file(GLOB_RECURSE KIWAKU_HEADERS ${CMAKE_SOURCE_DIR}/include/kiwaku/*.hpp)

# Setup doxygen
set(DOXYGEN_INPUT_DIR   ${PROJECT_SOURCE_DIR}/include/kiwaku)
set(DOXYGEN_OUTPUT_DIR  ${CMAKE_CURRENT_BINARY_DIR}/doxygen)
set(DOXYGEN_INDEX_FILE  ${DOXYGEN_OUTPUT_DIR}/xml/index.xml)
set(DOXYFILE_IN         ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
set(DOXYFILE_OUT        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
set(DOXYGEN_EXAMPLE_DIR "${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/test")

# Replace variables inside @@ with the current values
configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)
file(MAKE_DIRECTORY ${DOXYGEN_OUTPUT_DIR})

# Only regenerate Doxygen when the Doxyfile or public headers change
add_custom_command( OUTPUT  ${DOXYGEN_INDEX_FILE}
                    DEPENDS ${KIWAKU_HEADERS}
                    COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
                    MAIN_DEPENDENCY ${DOXYFILE_OUT} ${DOXYFILE_IN}
                    COMMENT "Extracting comments"
                    VERBATIM
                  )

add_custom_target(kiwaku-doxygen ALL DEPENDS ${DOXYGEN_INDEX_FILE})

set(SPHINX_HTML_DIR   "${CMAKE_SOURCE_DIR}/docs")
set(SPHINX_SOURCE     "${CMAKE_CURRENT_SOURCE_DIR}")
set(SPHINX_INDEX_FILE "${SPHINX_HTML_DIR}/index.html")

configure_file( "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in" "${CMAKE_CURRENT_SOURCE_DIR}/conf.py" @ONLY)

add_custom_command( OUTPUT  ${SPHINX_INDEX_FILE}
                    COMMAND ${SPHINX_EXECUTABLE}  -q -b html
                                                  -Dbreathe_projects.kiwaku=${DOXYGEN_OUTPUT_DIR}/xml
                                                  ${SPHINX_SOURCE}
                                                  ${SPHINX_HTML_DIR}
                    WORKING_DIRECTORY ${SPHINX_HTML_DIR}
                    DEPENDS
                    ${DOXYGEN_INDEX_FILE}
                    ${CMAKE_CURRENT_SOURCE_DIR}/howto.rst
                    ${CMAKE_CURRENT_SOURCE_DIR}/index.rst
                    ${CMAKE_CURRENT_SOURCE_DIR}/install.rst
                    ${CMAKE_CURRENT_SOURCE_DIR}/reference.rst
                    ${CMAKE_CURRENT_SOURCE_DIR}/_static/css/custom.css
                    ${CMAKE_CURRENT_SOURCE_DIR}/_static/logo.png
                    MAIN_DEPENDENCY ${CMAKE_CURRENT_SOURCE_DIR}/conf.py
                    COMMENT "Generating documentation with Sphinx"
                  )
add_custom_target(doc ALL DEPENDS ${SPHINX_INDEX_FILE})

else()
    message(STATUS "[kiwaku] Sphinx not found - Disabling documentation")
endif()



