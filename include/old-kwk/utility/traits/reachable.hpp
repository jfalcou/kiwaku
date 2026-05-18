//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kwk/concepts/slicer.hpp>
#include <kwk/detail/stdfix.hpp>

namespace kwk
{
  template<typename... Slicers>
  inline constexpr bool preserve_reachability = false;

  // 1D cases - everything but step is contiguous
  template<concepts::slicer Slicer>
  inline constexpr bool preserve_reachability<Slicer> = concepts::unit_slicer<Slicer>;

  // 2D cases
  template<concepts::joker S1, concepts::joker S0>
  inline constexpr bool preserve_reachability<S1,S0>  =  true;

  template<std::integral S1, concepts::slicer S0>
  inline constexpr bool preserve_reachability<S1,S0> = preserve_reachability<S0>;

  template<concepts::extremum S1, concepts::slicer S0>
  inline constexpr bool preserve_reachability<S1,S0> = preserve_reachability<S0>;

  template<concepts::unit_interval_slicer S1, concepts::joker S0>
  inline constexpr bool preserve_reachability<S1,S0>  =  true;

  // nD cases - check all first then go to 2D
  template< concepts::unit_slicer S, concepts::unit_slicer T, concepts::unit_slicer U
          , typename... Slicers
          >
  inline constexpr bool preserve_reachability<S,T,U,Slicers...> =  preserve_reachability<Slicers...>;
}
