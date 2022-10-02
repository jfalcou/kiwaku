##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      as_array.cpp
      # as_stride.cpp
      compare.cpp
      contain.cpp
      convert.cpp
      ctor.cpp
      default_ctor.cpp
      joker.cpp
      properties.cpp
      of_size.cpp
      mixed.cpp
      strictly_contain.cpp
      swap.cpp
      tuple.cpp
    )

make_unit("utility.shape" ${SOURCES})
