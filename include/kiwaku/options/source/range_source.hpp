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

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    struct range_span
    {
      T* data;
      constexpr auto get()  const noexcept { return data; }
    };

    constexpr auto as_span()        const noexcept { return range_span{data}; }
    constexpr auto default_shape()  const noexcept { return shape<_1D>(size); }

    T*              data;
    std::ptrdiff_t  size;
  };
}
