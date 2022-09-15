##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      ctor.cpp
      indexing.cpp
      unit_indexing.cpp
    )

make_unit("components.stride" ${SOURCES})
