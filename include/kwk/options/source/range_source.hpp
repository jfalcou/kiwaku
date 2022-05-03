//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <kwk/container/shape.hpp>

namespace kwk
{
  struct data_source;

  template<typename T, typename Size> struct range_source
  {
    using stored_value_type = range_source<T,Size>;
    using keyword_type      = data_source;

    constexpr range_source(T* p = nullptr, Size s = 0) : data_(p), size_(s) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    struct range_span
    {
      static constexpr bool own_data = false;

      using base_type = T;
      T* data_;
      constexpr auto data()  const noexcept { return data_; }
    };

    using span_type         = range_span;

    constexpr auto as_block(std::ptrdiff_t offset = 0) const noexcept
    {
      return range_span{data_ - offset};
    }

    constexpr auto default_shape()  const noexcept { return of_size(size_);     }

    T*    data_;
    Size  size_;
  };
}
