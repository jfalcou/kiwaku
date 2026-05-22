//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk::__
{
  struct default_allocator
  {
    using element_type = void;
    using type = default_allocator;
    using identifier_type = default_allocator;
    using label_type = kumi::str;

    constexpr auto operator()(identifier_type const&) const { return *this; }

    static constexpr label_type label() { return kumi::str{"Deduced Allocator"}; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         default_allocator const&) noexcept
    {
      return os << "Deduced Allocator";
    }
  };

  inline constexpr default_allocator deduce_allocator{};
}
