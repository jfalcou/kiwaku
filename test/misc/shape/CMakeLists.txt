##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      as_stride.cpp
      compare.cpp
      contain.cpp
      convert.cpp
      ctor.cpp
      default.cpp
      ndims.cpp
      numel.cpp
      of_shape.cpp
      strictly_contain.cpp
      swap.cpp
      tuple.cpp
      experimental.cpp
    )

make_unit("misc.shape" ${SOURCES})
