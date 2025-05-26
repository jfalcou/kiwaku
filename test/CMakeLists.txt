##==================================================================================================
##  KIWAKU - Containers Well Made
##  Copyright : KIWAKU Contributors & Maintainers
##  SPDX-License-Identifier: BSL-1.0
##==================================================================================================
include(make_unit)
set(unit_root "${CMAKE_SOURCE_DIR}/test")

# SYCL

glob_unit(${unit_root} "algorithm/algos/context/sycl/*.cpp")

glob_unit(${unit_root} "algorithm/algos/*.cpp")

glob_unit(${unit_root} "algorithm/algos/reduce/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/reduce/*.cpp")        # CPU context
glob_unit(${unit_root} "algorithm/algos/context/sycl/reduce/*.cpp")       # SYCL context

glob_unit(${unit_root} "algorithm/algos/copy/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/copy/*.cpp")          # CPU context

glob_unit(${unit_root} "algorithm/algos/numeric/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/numeric/*.cpp")       # CPU context

glob_unit(${unit_root} "algorithm/algos/replace/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/replace/*.cpp")       # CPU context

glob_unit(${unit_root} "algorithm/algos/binary_search/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/binary_search/*.cpp") # CPU context

glob_unit(${unit_root} "algorithm/algos/find/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/find/*.cpp")          # CPU context
glob_unit(${unit_root} "algorithm/algos/context/sycl/find/*.cpp")          # SYCL context

glob_unit(${unit_root} "algorithm/algos/predicates/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/predicates/*.cpp")    # CPU context

glob_unit(${unit_root} "algorithm/algos/generator/*.cpp")
glob_unit(${unit_root} "algorithm/algos/context/cpu/generator/*.cpp")     # CPU context

# Context-specific tests
glob_unit(${unit_root} "algorithm/algos/context/cpu/*.cpp")


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
