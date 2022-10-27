//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once
#include <kwk/utility/container/shape.hpp>
#include <kwk/detail/memory/shallow_block.hpp>

namespace kwk::detail { struct source_; }

namespace kwk
{
  template<typename T> struct ptr_source
  {
    using stored_value_type = ptr_source<T>;
    using keyword_type      = detail::source_;

    using value_type      = std::remove_const_t<T>;
    using reference       = std::add_lvalue_reference_t<T>;
    using const_reference = std::add_lvalue_reference_t<T const>;
    using pointer         = std::add_pointer_t<T>;
    using const_pointer   = std::add_pointer_t<T const>;

    constexpr ptr_source(T* p = nullptr) : data_(p) {}

    template<typename OtherSource>
    constexpr ptr_source(OtherSource p = {}) : data_(data(p)) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    T* data_;
  };

  template<typename T>
  constexpr auto storage(ptr_source<T> src) noexcept
  {
    return detail::shallow_block{src.data_};
  }

  template<typename T>
  constexpr auto default_shape(ptr_source<T> const&)  noexcept { return shape<_1D>{0};  }

  template<typename T> struct source_traits;
  template<typename T> struct source_traits<ptr_source<T>>
  {
    using value_type = T;
  };
}
