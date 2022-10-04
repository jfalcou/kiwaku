//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/utility/joker.hpp>
#include <kwk/detail/kumi.hpp>

namespace kwk
{
  template<std::size_t N, auto Desc>
  inline constexpr bool is_dynamic_extent_v = is_joker_v<kumi::element_t<N,decltype(Desc)>>;

  template<std::size_t N, auto Desc>
  inline constexpr bool is_static_extent_v = !is_dynamic_extent_v<N,Desc>;

  template<std::size_t N, auto Desc>
  struct is_dynamic_extent : std::bool_constant<is_dynamic_extent_v<N,Desc>>
  {};

  template<std::size_t N, auto Desc>
  struct is_static_extent : std::bool_constant<is_static_extent_v<N,Desc>>
  {};
}
