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
  template<typename T, std::ptrdiff_t Size>
  struct stack_block
  {
    using base_type = std::remove_const_t<T>;

    base_type data_[Size];

    constexpr auto data()       noexcept { return &data_[0]; }
    constexpr auto data() const noexcept { return &data_[0]; }

    constexpr void swap(stack_block& other) noexcept
    {
      for(std::size_t i=0;i<Size;++i)
        std::swap(data_[i], other.data_[i]);
    }
  };
}
