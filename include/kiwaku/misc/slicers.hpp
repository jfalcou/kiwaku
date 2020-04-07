//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_MISC_SLICERS_HPP_INCLUDED
#define KIWAKU_MISC_SLICERS_HPP_INCLUDED

#include <kiwaku/detail/slicers/single_slice.hpp>
#include <kiwaku/detail/slicers/full_slicer.hpp>
#include <kiwaku/detail/slicers/unit_slicer.hpp>
#include <kiwaku/detail/slicers/slicer.hpp>
#include <kiwaku/detail/slicers/axis.hpp>
#include <cstddef>

namespace kwk
{
  inline constexpr detail::full_slicer _ = {};
}

#endif
