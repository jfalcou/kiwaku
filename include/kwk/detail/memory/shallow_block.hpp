//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once
#include <type_traits>

namespace kwk::detail
{
  template<typename T> struct shallow_block
  {
    using base_type       = T;
    using value_type      = std::remove_const_t<base_type>;
    using reference       = std::add_lvalue_reference_t<base_type>;
    using const_reference = std::add_lvalue_reference_t<base_type const>;
    using pointer         = std::add_pointer_t<base_type>;
    using const_pointer   = std::add_pointer_t<base_type const>;

    pointer data_;

    constexpr shallow_block( pointer p) : data_ (p) {}

    constexpr         void swap(shallow_block& o) noexcept  { std::swap(data_, o.data_); }
    constexpr friend  void swap(shallow_block& a,shallow_block& b)  noexcept { a.swap(b); }

    constexpr auto get_data()       noexcept { return data_; }
    constexpr auto get_data() const noexcept { return data_; }
  };

  template<typename T> shallow_block( T* p) -> shallow_block<T>;

  template<typename T>
  constexpr auto data(shallow_block<T> const& src) noexcept { return src.get_data(); }

  template<typename T>
  constexpr auto data(shallow_block<T>& src) noexcept { return src.get_data(); }

}
