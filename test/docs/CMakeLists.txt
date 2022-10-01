##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      shape/compatible.cpp
      #shape/odd_sized.cpp
      shape/mixed.cpp
    )

make_unit("docs" ${SOURCES})
