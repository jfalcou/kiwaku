##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      ctor.cpp
      indexing.cpp
      unit_indexing.cpp
    )

make_unit("components.stride" ${SOURCES})
