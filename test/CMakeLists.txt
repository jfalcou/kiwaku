##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
copa_setup_test_targets()
set(root "${CMAKE_SOURCE_DIR}/test")

copa_glob_unit( PATTERN "unit/*.cpp"
                QUIET RELATIVE ${root} INTERFACE kiwaku_test DEPENDENCIES kiwaku-standalone
              )

copa_glob_unit( PATTERN "docs/*.cpp"
                QUIET RELATIVE ${root} INTERFACE kiwaku_test DEPENDENCIES kiwaku-standalone
              )
