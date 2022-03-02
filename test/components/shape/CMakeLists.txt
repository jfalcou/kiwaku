##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      as_stride.cpp
      compare.cpp
      contain.cpp
      convert.cpp
      ctor.cpp
      properties.cpp
      of_shape.cpp
      mixed.cpp
      strictly_contain.cpp
      swap.cpp
      tuple.cpp
    )

make_unit("components.shape" ${SOURCES})
