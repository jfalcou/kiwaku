##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
copa_setup_test_targets()
set(root "${CMAKE_SOURCE_DIR}/test")

##==================================================================================================
## Unit Tests
##==================================================================================================
copa_glob_unit( PATTERN "unit/utility/memory/*.cpp"
                QUIET RELATIVE ${root} INTERFACE kiwaku_test
              )

##==================================================================================================
## Doc Tests
##==================================================================================================
# copa_glob_unit( PATTERN "docs/tutorial/*.cpp"
#                 QUIET RELATIVE ${root} INTERFACE kiwaku_test
#               )
