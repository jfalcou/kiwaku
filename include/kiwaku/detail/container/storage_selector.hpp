//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_STORAGE_SELECTOR_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_STORAGE_SELECTOR_HPP_INCLUDED

#include <kiwaku/detail/container/stack_storage.hpp>
#include <kiwaku/detail/container/dynamic_storage.hpp>
#include <type_traits>

namespace kwk::detail
{
  template<typename Type, auto... Settings>
  struct storage_selector
  {
    static constexpr auto all_settings  = settings(Settings...);
    static constexpr auto shape         = options::shape(all_settings);
    static constexpr auto stride        = options::stride(all_settings);
    static constexpr auto storage       = options::storage(all_settings);

    static constexpr bool use_allocator = storage.use_allocator( shape.numel() * sizeof(Type) );

    using type = std::conditional_t < use_allocator
                                    , dynamic_storage<Type,Settings...>
                                    , stack_storage<Type,shape,stride>
                                    >;
  };
}

#endif
