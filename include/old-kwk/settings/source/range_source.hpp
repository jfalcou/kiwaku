//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once
#include <kwk/utility/container/shape.hpp>

namespace kwk::__ { struct source_; }

namespace kwk
{
  template<typename T> struct range_source
  {
    using stored_value_type = range_source<T>;
    using keyword_type      = __::source_;

    constexpr range_source(T* p = nullptr, std::size_t s = 0) : data_(p), size_(s) {}
    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    T*          data_;
    std::size_t size_;
  };

  template<typename T>
  constexpr auto storage(range_source<T> src) noexcept
  {
    return src.data_;
  }

  template<typename T>
  constexpr auto default_shape(range_source<T> const& src)  noexcept
  {
    return of_size(src.size_);
  }

  template<typename T> struct source_traits;
  template<typename T> struct source_traits<range_source<T>>
  {
    using value_type = T;
  };
}
