//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <type_traits>

namespace kwk::detail
{
  template<typename T, std::ptrdiff_t Size, std::ptrdiff_t Offset>
  struct stack_block
  {
    using base_type       = T;
    using value_type      = std::remove_const_t<base_type>;
    using reference       = std::add_lvalue_reference_t<base_type>;
    using const_reference = std::add_lvalue_reference_t<base_type const>;
    using pointer         = std::add_pointer_t<base_type>;
    using const_pointer   = std::add_pointer_t<base_type const>;

    value_type data_[Size];

    constexpr auto data()       noexcept { return &data_[0] - Offset; }
    constexpr auto data() const noexcept { return &data_[0] - Offset; }

    constexpr void swap(stack_block& other) noexcept
    {
      for(std::size_t i=0;i<Size;++i)
        std::swap(data_[i], other.data_[i]);
    }
  };
}
