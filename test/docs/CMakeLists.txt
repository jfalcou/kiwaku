##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      shape/compatible.cpp
      shape/odd_sized.cpp
      shape/mixed.cpp
    )

make_unit("docs" ${SOURCES})
