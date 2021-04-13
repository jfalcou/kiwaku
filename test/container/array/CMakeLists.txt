##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright 2020 Joel FALCOU
##
##  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
##  SPDX-License-Identifier: MIT
##==================================================================================================
set ( SOURCES
      dynamic.1d.cpp
      dynamic.2d.cpp
      static.1d.cpp
      static.2d.cpp
      static.3d.cpp
      static.4d.cpp
    )

make_unit("container.array" ${SOURCES})
