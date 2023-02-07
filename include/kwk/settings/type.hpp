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
      using type = T;

      constexpr auto operator()(keyword_type const&) const noexcept { return *this; }
      friend std::ostream& operator<<(std::ostream& os, info const&)
      {
        return os << rbr::detail::type_name<T>();
      }
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
}

namespace kwk
{
  constexpr inline __::type_ type{};

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Type setting for kwk::table
  //================================================================================================
  template<typename T> constexpr inline __::type_::info<T> as{};

  /// Pre-defined type settings for float
  constexpr inline auto single_ = as<float>;

  /// Pre-defined type settings for double
  constexpr inline auto double_ = as<double>;

  /// Pre-defined type settings for std::int8_t
  constexpr inline auto int8_   = as<std::int8_t>;

  /// Pre-defined type settings for std::int16_t
  constexpr inline auto int16_  = as<std::int16_t>;

  /// Pre-defined type settings for std::int32_t
  constexpr inline auto int32_  = as<std::int32_t>;

  /// Pre-defined type settings for std::int64_t
  constexpr inline auto int64_  = as<std::int64_t>;

  /// Pre-defined type settings for std::uint8_t
  constexpr inline auto uint8_  = as<std::uint8_t>;

  /// Pre-defined type settings for std::uint16_t
  constexpr inline auto uint16_ = as<std::uint16_t>;

  /// Pre-defined type settings for std::uint32_t
  constexpr inline auto uint32_ = as<std::uint32_t>;

  /// Pre-defined type settings for std::uint64_t
  constexpr inline auto uint64_ = as<std::uint64_t>;
}
