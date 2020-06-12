//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <kiwaku/detail/container/storage_selector.hpp>

namespace kwk
{
  template<typename Type, auto... Settings>
  struct array : private detail::storage_selector<Type,Settings...>::type
  {
    using storage                       = detail::storage_selector<Type,Settings...>;
    using parent                        = typename storage::type;
    static constexpr bool is_dynamic    = parent::is_dynamic;
    static constexpr bool use_allocator = storage::use_allocator;

    using shape_type = typename parent::shape_type;

    array() : parent{} {}
    array(shape_type const& s) noexcept requires(use_allocator) : parent{s} {}

    void swap( array& that ) noexcept
    {
      parent::swap( static_cast<parent&>(that) );
    }
/*
    template<typename SomeShape> void resize(SomeShape const& s)
    {
      storage_.resize(s.numel());
      view_ = view{storage_.data(),shape_type(s)};
    }

    template<typename SomeShape> void reshape(SomeShape const& s)
    {
      assert( s.numel() == view_.size() && "[array::reshape] - Incompatible shape");
      view_ = view{storage_.data(),shape_type(s)};
    }
*/

    auto as_view() const { return view{data(),shape()}; }

    using parent::size;
    using parent::count;
    using parent::data;
    using parent::begin;
    using parent::end;
    using parent::shape;
    using parent::stride;
    using parent::operator();
  };
}
