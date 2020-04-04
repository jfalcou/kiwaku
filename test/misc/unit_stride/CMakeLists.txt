##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      ctor.cpp
      value_ctor.cpp
      index.cpp
    )

make_unit("misc.unit_stride" ${SOURCES})
