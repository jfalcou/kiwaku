##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      allocator.cpp
      fixed.cpp
      heap.cpp
    )

make_unit("utility" ${SOURCES})
