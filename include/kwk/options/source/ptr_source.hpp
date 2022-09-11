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

  template<typename T> struct ptr_source
  {
    using base_type         = T;
    using span_type         = ptr_source;
    using stored_value_type = ptr_source<T>;
    using keyword_type      = data_source;

    using value_type      = std::remove_const_t<base_type>;
    using reference       = std::add_lvalue_reference_t<base_type>;
    using const_reference = std::add_lvalue_reference_t<base_type const>;
    using pointer         = std::add_pointer_t<base_type>;
    using const_pointer   = std::add_pointer_t<base_type const>;

    constexpr ptr_source(T* p = nullptr) : data_(p) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    constexpr auto as_block(std::ptrdiff_t offset = 0) const noexcept
    {
      return ptr_source{data_ - offset};
    }

    constexpr pointer reset(pointer ptr) noexcept
    {
      return std::exchange(data_, ptr);
    }

    constexpr auto data()           const noexcept { return data_;          }
    constexpr auto default_shape()  const noexcept { return shape<_1D>{0};  }
    T* data_;
  };
}
