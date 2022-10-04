//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
**/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <cstdint>

namespace kwk::detail
{
  struct type_ : rbr::as_keyword<type_>
  {
    template<typename T>
    struct info
    {
      using stored_value_type = info<T>;
      using keyword_type      = type_;
      using type = T;

      constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
    };

    template<typename T> constexpr auto operator=(info<T> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename T> std::ostream& display(std::ostream& os, info<T> const&) const
    {
      return os << "Type: " << rbr::detail::type_name<T>();
    }
  };
}

namespace kwk
{
  //================================================================================================
  //! @ingroup  settings
  //! @brief    Type setting for kwk::table
  //================================================================================================
  template<typename T> constexpr inline detail::type_::info<T> type{};

  /// Pre-defined type settings for float
  constexpr inline auto single_ = type<float>;

  /// Pre-defined type settings for double
  constexpr inline auto double_ = type<double>;

  /// Pre-defined type settings for std::int8_t
  constexpr inline auto int8_   = type<std::int8_t>;

  /// Pre-defined type settings for std::int16_t
  constexpr inline auto int16_  = type<std::int16_t>;

  /// Pre-defined type settings for std::int32_t
  constexpr inline auto int32_  = type<std::int32_t>;

  /// Pre-defined type settings for std::int64_t
  constexpr inline auto int64_  = type<std::int64_t>;

  /// Pre-defined type settings for std::uint8_t
  constexpr inline auto uint8_  = type<std::uint8_t>;

  /// Pre-defined type settings for std::uint16_t
  constexpr inline auto uint16_ = type<std::uint16_t>;

  /// Pre-defined type settings for std::uint32_t
  constexpr inline auto uint32_ = type<std::uint32_t>;

  /// Pre-defined type settings for std::uint64_t
  constexpr inline auto uint64_ = type<std::uint64_t>;
}
