##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================

##==================================================================================================
## Reuse install.cmake to prepare package properly
##==================================================================================================
get_filename_component(SELF_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${SELF_DIR}/kiwaku_lib.cmake)
set(KIWAKU_LIBRARIES kiwaku_lib)
