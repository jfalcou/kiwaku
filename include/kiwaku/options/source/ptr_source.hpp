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

  template<typename T> struct ptr_source
  {
    using base_type         = T;
    using span_type         = ptr_source;
    using stored_value_type = ptr_source<T>;
    using keyword_type      = data_source;

    constexpr ptr_source(T* p = nullptr) : data_(p) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    constexpr auto as_span()        const noexcept { return *this;          }
    constexpr auto data()           const noexcept { return data_;          }
    constexpr auto default_shape()  const noexcept { return shape<_1D>{0};  }
    T* data_;
  };
}
