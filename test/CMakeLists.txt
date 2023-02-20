##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
include(make_unit)
set(unit_root "${CMAKE_SOURCE_DIR}/test")

glob_unit(${unit_root} "concepts/*.cpp")
glob_unit(${unit_root} "container/table/*.cpp")
glob_unit(${unit_root} "container/view/*.cpp")
glob_unit(${unit_root} "docs/shape/*.cpp")
glob_unit(${unit_root} "docs/tutorial/*.cpp")
glob_unit(${unit_root} "utility/interpolator/*.cpp")
glob_unit(${unit_root} "utility/memory/*.cpp")
glob_unit(${unit_root} "utility/meta/*.cpp")
glob_unit(${unit_root} "utility/shape/*.cpp")
glob_unit(${unit_root} "utility/slicer/*.cpp")
glob_unit(${unit_root} "utility/stride/*.cpp")
