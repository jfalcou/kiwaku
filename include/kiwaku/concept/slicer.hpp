//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <type_traits>

namespace kwk::detail
{
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;
}

namespace kwk::concepts
{
  template<typename T, int D>
  concept contiguous_slicer = detail::is_contiguous_slicer<std::remove_cvref_t<T>,D>::value;

  template<typename T>
  concept unit_slicer = detail::is_unit_slicer<std::remove_cvref_t<T>>::value;
}
