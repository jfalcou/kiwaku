//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#ifndef KIWAKU_DETAIL_OPTIONS_STORAGE_OPTION_HPP_INCLUDED
#define KIWAKU_DETAIL_OPTIONS_STORAGE_OPTION_HPP_INCLUDED

#include <kiwaku/detail/options/options.hpp>

namespace kwk::detail
{
  struct adaptative_storage_option
  {
    using option_tag  = storage_tag;
    constexpr bool use_heap(std::ptrdiff_t sz) const noexcept
    {
      // negative sz means we deals with a full dynamic shape - so we use the heap
      return sz<0 || sz > heap_storage_threshold_;
    }

    std::ptrdiff_t heap_storage_threshold_;
  };

  struct heap_storage_option
  {
    using option_tag  = storage_tag;
    constexpr bool use_heap(std::ptrdiff_t) const noexcept { return true; }
    constexpr adaptative_storage_option operator()(std::ptrdiff_t t) noexcept { return {t}; }
  };

  struct stack_storage_option
  {
    using option_tag  = storage_tag;
    constexpr bool use_heap(std::ptrdiff_t) const noexcept { return false; }
    constexpr adaptative_storage_option operator()(std::ptrdiff_t t) noexcept { return {t}; }
  };
}

namespace kwk
{
  inline constexpr auto heap_   = detail::heap_storage_option{};
  inline constexpr auto stack_  = detail::stack_storage_option{};
}

#endif
