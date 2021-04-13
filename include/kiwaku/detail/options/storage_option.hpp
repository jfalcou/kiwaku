//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright 2020 Joel FALCOU

  Licensed under the MIT License <http://opensource.org/licenses/MIT>.
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kiwaku/detail/raberu.hpp>
#include <kiwaku/allocator/heap_allocator.hpp>

namespace kwk::detail
{
  struct heap_storage_option
  {
    using allocator_type = heap_allocator;
    constexpr bool use_allocator() const noexcept { return true; }
  };

  struct stack_storage_option
  {
    using allocator_type = void;
    constexpr bool use_allocator() const noexcept { return false; }
  };

  //================================================================================================
  // RBR option global tag
  //================================================================================================
  struct storage_tag;
}

namespace kwk
{
  inline constexpr auto heap_   = detail::heap_storage_option{};
  inline constexpr auto stack_  = detail::stack_storage_option{};
}

//================================================================================================
// Register a RBR keyword
//================================================================================================
namespace kwk::option
{
  inline constexpr auto storage  = ::rbr::keyword<kwk::detail::storage_tag>;
}

namespace rbr
{
  //================================================================================================
  // Register as RBR option
  //================================================================================================
  template<> struct tag<kwk::detail::heap_storage_option>   : tag<kwk::detail::storage_tag> {};
  template<> struct tag<kwk::detail::stack_storage_option>  : tag<kwk::detail::storage_tag> {};
}
