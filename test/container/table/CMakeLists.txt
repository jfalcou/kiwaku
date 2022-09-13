##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      static.1d.cpp
      static.2d.cpp
      static.3d.cpp
      static.4d.cpp
      dynamic.1d.cpp
      dynamic.2d.cpp
      dynamic.3d.cpp
      dynamic.4d.cpp
    )

make_unit("container.table" ${SOURCES})
