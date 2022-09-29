##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      allocator.cpp
      concepts.cpp
      heap.cpp
    )

make_unit("utility.memory" ${SOURCES})
