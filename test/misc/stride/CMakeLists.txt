##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      from_shape.cpp
      indexing.cpp
      unit_indexing.cpp
      with_stride.cpp
    )

make_unit("misc.stride" ${SOURCES})
