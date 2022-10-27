//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <kwk/detail/memory/heap_block.hpp>
#include <kwk/detail/memory/shallow_block.hpp>
#include <kwk/detail/memory/stack_block.hpp>

namespace kwk::detail
{
  template<auto Kind, auto Shape, typename T, rbr::concepts::settings Settings>
  struct block_type;

  template<auto Shape, typename T, rbr::concepts::settings Settings>
  struct block_type<kwk::view_,Shape,T,Settings>
  {
    using type = shallow_block<T>;
  };

  template<auto Shape, typename T, rbr::concepts::settings Settings>
  struct block_type<kwk::table_,Shape,T,Settings>
  {
    static constexpr auto find() noexcept
    {
      if constexpr( Shape.is_fully_static )  return stack_block<T, Shape.numel()>{};
      else                                   return heap_block<T>{};
    }

    using type = decltype( find() );
  };

  template<auto Kind, auto Shape, typename T, rbr::concepts::settings Settings>
  using block_t = typename block_type<Kind, Shape, T, Settings>::type;
}
