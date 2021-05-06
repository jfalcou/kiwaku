//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

namespace kwk::concepts
{
  template<typename>      struct is_unit_slicer;
  template<typename,int>  struct is_contiguous_slicer;

  template<typename T, int D>
  concept contiguous_slicer = detail::is_contiguous_slicer<std::remove_cvref_t<T>,D>::value;

  template<typename T>
  concept unit_slicer = detail::is_unit_slicer<std::remove_cvref_t<T>>::value;
}
