//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/container/pick.hpp>
#include <kwk/detail/raberu.hpp>
#include <kwk/detail/memory/heap_block.hpp>
#include <kwk/detail/memory/stack_block.hpp>

namespace kwk::detail
{
  // For view, we compute the data_block from the source
  template<typename Shape, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto block(view_ const&, Shape const&, Settings const& p) noexcept
  {
    return storage(pick(source,p));
  }

  // For view, we compute the data_block from the source
  template<typename Shape, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto block(table_ const&, Shape const& s, Settings const& p) noexcept
  {
    using type  = typename result::pick_t<kwk::type,Settings>::type;

    auto data = [&]()
    {
      if constexpr( Shape::is_fully_static )  return stack_block<type, Shape{}.numel()>{};
      else                                    return heap_block<type> { pick(kwk::allocator,p)
                                                                      , s.numel()
                                                                      };
    }();

    if constexpr( Settings::contains(kwk::source) )
      data.assign( storage(pick(kwk::source, p)), s.numel() );

    return data;
  }

  // Retrieve the data block constructed from the settings
  template<typename Shape, rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto block(Shape const& s, Settings const& opts)
  requires( Settings::contains(category).value )
  {
    return block(opts[category], s, opts);
  }

  template<rbr::concepts::settings Settings>
  KWK_FORCEINLINE constexpr auto block(Settings const& opts)
  requires( Settings::contains(category).value )
  {
    return block(opts[category], opts[size], opts);
  }

  template<auto Shape, rbr::concepts::settings Settings>
  struct block_type
  {
    using type = decltype( block(Shape, std::declval<Settings>() ));
  };

  template<auto Shape, rbr::concepts::settings Settings>
  using block_type_t = typename block_type<Shape, Settings>::type;
}
