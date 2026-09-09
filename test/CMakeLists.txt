##======================================================================================================================
##  KIWAKU - Shape Up Your Data
##  Copyright : KIWAKU Project Contributors
##  SPDX-License-Identifier: BSL-1.0
##======================================================================================================================
copa_setup_test_targets()

copa_glob_unit(QUIET PATTERN "unit/*.cpp" INTERFACE kiwaku_tests)
