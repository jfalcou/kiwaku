##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      api.cpp
      all.cpp
      to.cpp
      from.cpp
      between.cpp
      every.cpp
    )

make_unit("utility.slicer" ${SOURCES})
