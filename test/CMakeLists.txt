##======================================================================================================================
##  KIWAKU - Shape Up Your Data
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
copa_setup_test_targets()
set(root "${CMAKE_SOURCE_DIR}/test")

copa_glob_unit(QUIET PATTERN "unit/*.cpp" INTERFACE kiwaku_test RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} )
