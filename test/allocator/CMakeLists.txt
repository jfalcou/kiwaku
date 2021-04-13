##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      allocator.cpp
      heap.cpp
      shallow.cpp
    )

make_unit("allocator" ${SOURCES})
