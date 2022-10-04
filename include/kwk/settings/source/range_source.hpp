//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <kwk/utility/container/shape.hpp>

namespace kwk
{
  struct source_;

  namespace detail
  {
    template<typename T> struct span
    {
      using value_type      = std::remove_const_t<T>;
      using reference       = std::add_lvalue_reference_t<T>;
      using const_reference = std::add_lvalue_reference_t<T const>;
      using pointer         = std::add_pointer_t<T>;
      using const_pointer   = std::add_pointer_t<T const>;

      T* data_;
    };
  }

  template<typename T> struct range_source
  {
    using stored_value_type = range_source<T>;
    using keyword_type      = source_;

    constexpr range_source(T* p = nullptr, std::size_t s = 0) : data_(p), size_(s) {}
    constexpr auto operator()(keyword_type const&) const noexcept { return *this; }

    T*          data_;
    std::size_t size_;
  };

  template<typename T>
  constexpr auto storage(range_source<T> src) noexcept
  {
    return typename detail::span<T>{src.data_};
  }

  template<typename T>
  constexpr auto reset(detail::span<T>& src, T* ptr) noexcept
  {
    return std::exchange(src.data_, ptr);
  }

  template<typename T>
  constexpr auto data(detail::span<T> const& src) noexcept { return src.data_; }

  template<typename T>
  constexpr auto default_shape(range_source<T> const& src)  noexcept
  {
    return of_size(src.size_);
  }

  template<typename T> struct source_traits;
  template<typename T> struct source_traits<range_source<T>>
  {
    using span_type = detail::span<T>;
    using base_type = T;
  };
}
