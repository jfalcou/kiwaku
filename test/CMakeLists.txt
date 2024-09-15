##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
include(make_unit)
set(unit_root "${CMAKE_SOURCE_DIR}/test")

glob_unit(${unit_root} "algorithm/algos/*.cpp")

glob_unit(${unit_root} "algorithm/algos/reduce/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/reduce.cpp")

glob_unit(${unit_root} "algorithm/algos/copy/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/copy.cpp")

glob_unit(${unit_root} "algorithm/algos/numeric/*.cpp")
# glob_unit(${unit_root} "algorithm/algos/context/cpu/numeric.cpp") WIP

glob_unit(${unit_root} "algorithm/views/*.cpp")
glob_unit(${unit_root} "concepts/*.cpp")
glob_unit(${unit_root} "container/table/*.cpp")
glob_unit(${unit_root} "container/view/*.cpp")
glob_unit(${unit_root} "docs/algorithms/*.cpp")
glob_unit(${unit_root} "docs/shape/*.cpp")
glob_unit(${unit_root} "docs/tutorial/*.cpp")
glob_unit(${unit_root} "utility/extensions/*.cpp")
glob_unit(${unit_root} "utility/interpolator/*.cpp")
glob_unit(${unit_root} "utility/memory/*.cpp")
glob_unit(${unit_root} "utility/meta/*.cpp")
glob_unit(${unit_root} "utility/shape/*.cpp")
glob_unit(${unit_root} "utility/slicer/*.cpp")
glob_unit(${unit_root} "utility/stride/*.cpp")
