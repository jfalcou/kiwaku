//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <kiwaku/components/shape.hpp>

namespace kwk
{
  struct data_source;

  template<typename T> struct range_source
  {
    using stored_value_type = range_source<T>;
    using keyword_type      = data_source;

    constexpr range_source(T* p = nullptr, std::ptrdiff_t s = 0) : data_(p), size_(s) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    struct range_span
    {
      using base_type = T;
      T* data_;
      constexpr auto data()  const noexcept { return data_; }
    };

    using span_type         = range_span;

    constexpr auto as_span()        const noexcept { return range_span{data_}; }
    constexpr auto default_shape()  const noexcept { return shape<_1D>(size_); }

    T*              data_;
    std::ptrdiff_t  size_;
  };
}
