##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================

##==================================================================================================
## Reuse install.cmake to preapre package properly
##==================================================================================================
include("${CMAKE_CURRENT_LIST_DIR}/kiwaku-targets.cmake")
set(KIWAKU_LIBRARIES kiwaku::kiwaku)
