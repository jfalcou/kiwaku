##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      as_array.cpp
      as_stride.cpp
      compare.cpp
      contain.cpp
      convert.cpp
      ctor.cpp
      joker.cpp
      properties.cpp
      of_shape.cpp
      mixed.cpp
      strictly_contain.cpp
      swap.cpp
      tuple.cpp
    )

make_unit("components.shape" ${SOURCES})
