//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#pragma once

#include <cstdint>
#include <iosfwd>
#include <kumi/kumi.hpp>
#include <kwk/detail/typename.hpp>

namespace kwk
{
  //================================================================================================
  /**
    @ingroup  settings
    @brief    Type setting for KIWAKU containers

    @tparam   T Type to be used within the container
    @see      as
  **/
  //================================================================================================
  template<typename T> struct type
  {
    /// The type to be used within the container
    using element_type = T;

    static constexpr auto option() { return (kumi::field<"type"> = type{}); }
  };

  template<typename C, typename R, typename T> auto& operator<<(std::basic_ostream<C, R>& os, type<T> const&)
  {
    return os << "type<" << __::name_of<T> << ">";
  }

  //================================================================================================
  /**
    @ingroup  settings
    @brief    Helper to create type settings for KIWAKU containers

    @tparam   T Type to be used within the container
    @see      type
  **/
  //================================================================================================
  template<typename T> constexpr inline type<T> as{};

  /// @brief Pre-defined type settings for float
  /// @relates type
  constexpr inline auto real32 = as<float>;

  /// @brief Pre-defined type settings for double
  /// @relates type
  constexpr inline auto real64 = as<double>;

  /// @brief  Pre-defined type settings for std::int8_t
  /// @relates type
  constexpr inline auto int8 = as<std::int8_t>;

  /// @brief Pre-defined type settings for std::int16_t
  /// @relates type
  constexpr inline auto int16 = as<std::int16_t>;

  /// @brief Pre-defined type settings for std::int32_t
  /// @relates type
  constexpr inline auto int32 = as<std::int32_t>;

  /// @brief Pre-defined type settings for std::int64_t
  /// @relates type
  constexpr inline auto int64 = as<std::int64_t>;

  /// @brief Pre-defined type settings for std::uint8_t
  /// @relates type
  constexpr inline auto uint8 = as<std::uint8_t>;

  /// @brief Pre-defined type settings for std::uint16_t
  /// @relates type
  constexpr inline auto uint16 = as<std::uint16_t>;

  /// @brief Pre-defined type settings for std::uint32_t
  /// @relates type
  constexpr inline auto uint32 = as<std::uint32_t>;

  /// @brief Pre-defined type settings for std::uint64_t
  /// @relates type
  constexpr inline auto uint64 = as<std::uint64_t>;
}
