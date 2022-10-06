##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
set ( SOURCES
      slicer_1D.cpp
      slicer_2D.cpp
      slicer_3D.cpp
      # slicer_4D.cpp
    )

make_unit("utility.slicer" ${SOURCES})
