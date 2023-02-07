//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <type_traits>
#include <algorithm>

namespace kwk::__
{
  template<typename T, std::ptrdiff_t Size>
  struct stack_block
  {
    using base_type       = T;
    using value_type      = std::remove_const_t<base_type>;
    using reference       = std::add_lvalue_reference_t<base_type>;
    using const_reference = std::add_lvalue_reference_t<base_type const>;
    using pointer         = std::add_pointer_t<base_type>;
    using const_pointer   = std::add_pointer_t<base_type const>;

    value_type data_[Size];

    constexpr stack_block() {}

    template<rbr::concepts::settings Settings>
    constexpr stack_block(Settings const& p)
    {
      if constexpr( Settings::contains(kwk::source) )
        assign( storage(pick(kwk::source, p)), pick(kwk::size,p).numel() );
    }

    constexpr auto get_data()       noexcept { return &data_[0]; }
    constexpr auto get_data() const noexcept { return &data_[0]; }

    constexpr void assign(auto const& src, auto)
    {
      std::copy(src, src + Size, get_data());
    }

    constexpr void swap(stack_block& other) noexcept
    {
      for(std::ptrdiff_t i=0;i<Size;++i)
        std::swap(data_[i], other.data_[i]);
    }
  };

  template<typename T, std::ptrdiff_t Size>
  constexpr auto data(stack_block<T,Size> const& src) noexcept { return src.get_data(); }

  template<typename T, std::ptrdiff_t Size>
  constexpr auto data(stack_block<T,Size>& src) noexcept { return src.get_data(); }
}
