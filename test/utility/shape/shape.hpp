//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <utility>

template<int N, typename Indexes = std::make_index_sequence<N>>  struct sizes;

template<int N, std::size_t... I> struct sizes<N, std::index_sequence<I...>>
{
  using types_list = tts::types<std::integral_constant<std::ptrdiff_t,I+1>...>;
};

