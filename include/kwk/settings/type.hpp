//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <cstdint>

namespace kwk::__
{
  struct type_ : rbr::as_keyword<type_>
  {
    template<typename T>
    struct info
    {
      using stored_value_type = info<T>;
      using keyword_type      = type_;
      using type              = T;

      constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
      friend std::ostream& operator<<(std::ostream& os, info const&)
      {
        return os << rbr::detail::type<T>.name();
      }

      friend constexpr bool operator==(info, info) noexcept { return true; }
      constexpr bool operator==(auto) noexcept { return false; }
    };

    template<typename T> constexpr auto operator=(info<T> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename T> std::ostream& display(std::ostream& os, info<T> const& s) const
    {
      return os << "Type: " << s;
    }
  };

  template<typename T> consteval auto extent(type_::info<T>) { return std::uint8_t{0}; } // see the note for joker?
}

namespace kwk
{
  constexpr inline __::type_ value_type{};

  template<typename T>
  using type = __::type_::info<T>;

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Type setting for kwk::table
  //================================================================================================
  template<typename T> constexpr inline type<T> as{};

  /// Pre-defined type settings for float
  constexpr inline auto real32  = as<float>;

  /// Pre-defined type settings for double
  constexpr inline auto real64  = as<double>;

  /// Pre-defined type settings for std::int8_t
  constexpr inline auto int8    = as<std::int8_t>;

  /// Pre-defined type settings for std::int16_t
  constexpr inline auto int16   = as<std::int16_t>;

  /// Pre-defined type settings for std::int32_t
  constexpr inline auto int32   = as<std::int32_t>;

  /// Pre-defined type settings for std::int64_t
  constexpr inline auto int64   = as<std::int64_t>;

  /// Pre-defined type settings for std::uint8_t
  constexpr inline auto uint8   = as<std::uint8_t>;

  /// Pre-defined type settings for std::uint16_t
  constexpr inline auto uint16  = as<std::uint16_t>;

  /// Pre-defined type settings for std::uint32_t
  constexpr inline auto uint32  = as<std::uint32_t>;

  /// Pre-defined type settings for std::uint64_t
  constexpr inline auto uint64  = as<std::uint64_t>;
}
