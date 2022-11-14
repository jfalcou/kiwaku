##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      as_stride.cpp
      with_strides.cpp
      linear_index.cpp
    )

make_unit("utility.stride" ${SOURCES})
