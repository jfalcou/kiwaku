//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once
#include <kiwaku/container/shape.hpp>

namespace kwk
{
  struct data_source;

  template<typename T, std::size_t N> struct array_source
  {
    using base_type         = T;
    using stored_value_type = array_source<T,N>;
    using keyword_type      = data_source;
    using span_type         = array_source<T,N>;

    constexpr array_source(T* p = nullptr) : data_(p) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    constexpr auto as_span()        const noexcept { return *this;              }
    constexpr auto data()           const noexcept { return data_;              }
    constexpr auto default_shape()  const noexcept { return of_size(fixed<N>);  }

    T*  data_;
  };
}
