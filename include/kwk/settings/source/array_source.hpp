//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once
#include <kwk/utility/container/shape.hpp>

namespace kwk::detail { struct source_; }

namespace kwk
{
  template<typename T, std::size_t N> struct array_source
  {
    using stored_value_type = array_source<T,N>;
    using keyword_type      = detail::source_;

    using value_type      = std::remove_const_t<T>;
    using reference       = std::add_lvalue_reference_t<T>;
    using const_reference = std::add_lvalue_reference_t<T const>;
    using pointer         = std::add_pointer_t<T>;
    using const_pointer   = std::add_pointer_t<T const>;

    constexpr array_source(T* p = nullptr) : data_(p) {}

    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    T*  data_;
  };

  template<typename T, std::size_t N>
  constexpr auto storage(array_source<T,N> const& src) noexcept
  {
    return src.data_;
  }

  template<typename T, std::size_t N>
  constexpr auto default_shape(array_source<T,N> const&)  noexcept { return of_size(fixed<N>);  }

  template<typename T> struct source_traits;
  template<typename T, std::size_t N> struct source_traits<array_source<T,N>>
  {
    using value_type = T;
  };
}
