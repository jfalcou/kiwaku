//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_CONTAINER_VIEW_SPAN_HPP_INCLUDED
#define KIWAKU_DETAIL_CONTAINER_VIEW_SPAN_HPP_INCLUDED

#include <kiwaku/detail/ct_helpers.hpp>
#include <iterator>
#include <cstddef>

namespace kwk::detail
{
  //================================================================================================
  // view_span is an helper base class that isolate all the data and functions related to
  // holding the actual pointer of the view.
  //================================================================================================
  template<typename Data> struct view_span;

  template<typename Type> struct view_span<Type*>
  {
    using base_type               = Type;
    using value_type              = std::remove_const_t<Type>;
    using reference               = std::add_lvalue_reference_t<base_type>;
    using const_reference         = std::add_lvalue_reference_t<base_type const>;
    using pointer                 = std::add_pointer_t<base_type>;
    using const_pointer           = std::add_pointer_t<base_type const>;
    using iterator                = pointer;
    using const_iterator          = const_pointer;
    using reverse_iterator        = std::reverse_iterator<iterator>;
    using const_reverse_iterator  = std::reverse_iterator<const_iterator>;

    view_span( pointer p )  noexcept : data_(p) {}
    pointer data() const noexcept { return data_; }

    protected:
    pointer data_;
  };
}

#endif
