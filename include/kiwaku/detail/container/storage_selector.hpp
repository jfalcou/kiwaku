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
#include <kiwaku/detail/options/storage_option.hpp>
#include <type_traits>

namespace kwk::detail
{
  template<typename Type, auto... Settings>
  struct storage_selector : settings_extractor<Type,Settings...>
  {
    using base_t = settings_extractor<Type,Settings...>;

    static constexpr auto storage_ = []()
    {
      if constexpr(!base_t::shape_.is_fully_static) return base_t::opt_[option::storage | dynamic_];
      else                                          return base_t::opt_[option::storage | stack_];
    }();

    static constexpr bool use_allocator = storage_.use_allocator();

    using type = std::conditional_t < use_allocator
                                    , dynamic_storage<Type,Settings...>
                                    , stack_storage<Type,base_t::shape_,base_t::stride_>
                                    >;
  };
}
#endif
