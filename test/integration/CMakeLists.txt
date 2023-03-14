##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
# Extract HEAD from git
find_package(Git REQUIRED)
execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
    OUTPUT_VARIABLE kiwaku_git_head
    RESULT_VARIABLE _git_error
    OUTPUT_STRIP_TRAILING_WHITESPACE)

if(NOT _git_error STREQUAL "0")
    message(SEND_ERROR
        "Failed to extract Kiwaku's current git commit for integration tests: ${_git_error}")
    return()
endif()


# Builds and tests a separate project as an EVE test
function(kiwaku_build_and_ctest project_name)
  string(REPLACE "-test" "" test_name "integration.${project_name}.exe")

  add_test(
    NAME ${test_name}
    COMMAND
    "${CMAKE_CTEST_COMMAND}"
    --verbose
    --output-on-failure
    --build-generator "${CMAKE_GENERATOR}"
    --build-and-test
      "${CMAKE_CURRENT_SOURCE_DIR}/${project_name}"
      "${CMAKE_CURRENT_BINARY_DIR}/${project_name}"
    --build-options ${ARGN}
    --test-command "${CMAKE_CTEST_COMMAND}")
endfunction()


##==================================================================================================
## Create Integration Tests
##==================================================================================================

## ===== CPM =====
kiwaku_build_and_ctest(cpm-test
    "-Dkiwaku_git_head=${kiwaku_git_head}"
    "-Dkiwaku_SOURCE_DIR=${kiwaku_SOURCE_DIR}"
    # cpm-test internally sets EVE_* configure options
)

## ===== Fetch =====
kiwaku_build_and_ctest(fetch-test
    "-Dkiwaku_git_head=${kiwaku_git_head}"
    "-Dkiwaku_SOURCE_DIR=${kiwaku_SOURCE_DIR}"
    "-DKIWAKU_BUILD_TEST=OFF"
    "-DKIWAKU_BUILD_DOCUMENTATION=OFF"
)

## ===== Install =====
# install eve as its own test and for 'install-test' to find
set(kiwaku_ROOT "${PROJECT_BINARY_DIR}/test/integration/install")
add_test(
  NAME integration.install-kwk.exe
  COMMAND
  "${CMAKE_COMMAND}"
  --install "${PROJECT_BINARY_DIR}"
  --verbose
  --prefix  "${kiwaku_ROOT}")

kiwaku_build_and_ctest(install-test "-Dkiwaku_ROOT=${kiwaku_ROOT}")
set_tests_properties(integration.install.exe PROPERTIES DEPENDS integration.install-kwk.exe)

## ==== Aggregated ====
add_custom_target(integration
  COMMAND
  "${CMAKE_CTEST_COMMAND}"
  --test-dir "${PROJECT_BINARY_DIR}"
  --output-on-failure
  -R integration\.[a-z-]+\.exe)
