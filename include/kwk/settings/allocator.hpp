//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

namespace kwk
{
  namespace _
  {
    template<typename T> struct allocator_option;

    struct allocator_id : kumi::identifier<allocator_id>
    {
      template<concepts::allocator T> constexpr auto operator=(T const& cnst) const noexcept
      {
        return allocator_option<T>{cnst};
      }

      template<typename T>
      requires(!concepts::allocator<T>)
      constexpr auto operator=(T const&) const = delete;

      friend constexpr auto to_str(allocator_id) { return kumi::str{"Allocator"}; }
    };

    template<typename T> struct allocator_option
    {
      using element_type = T;
      using type = allocator_option<T>;
      using identifier_type = allocator_id;

      T value;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Allocator"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           allocator_option const&) noexcept
      {
        return os << "Allocator: " << kumi::_::typer<T>();
      }
    };
  }

  /**
    @ingroup  settings-allocator
    @brief Identifier for the allocator setting for containers

    Identifies the allocator setting for KIWAKU containers, which specifies the allocator used by the container.
  **/
  inline constexpr _::allocator_id allocator{};
}
