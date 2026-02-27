//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <cstdint>
#include <kumi/kumi.hpp>

namespace kwk
{
  //====================================================================================================================
  /**
    @ingroup  settings
    @brief    Kind setting for KIWAKU containers

    @tparam   T Type to be used within the container
  **/
  //====================================================================================================================
  template<typename T> struct kind : kumi::identifier<kind<T>>
  {
    /// The type to be used within the container
    using element_type = T;
    using type = kind<T>;
    using identifier_type = kind<T>;

    constexpr auto operator()(identifier_type) const { return *this; }

    static constexpr auto name() { return kumi::_::typer<identifier_type>(); }
  };

  //====================================================================================================================
  /**
    @ingroup  settings
    @brief    Helper to create type settings for KIWAKU containers

    @tparam   T Type to be used within the container
    @relates  kind
  **/
  //====================================================================================================================
  template<typename T> constexpr inline kind<T> as{};

  /// @brief Pre-defined type settings for float
  /// @relates kind
  constexpr inline auto real32 = as<float>;

  /// @brief Pre-defined type settings for double
  /// @relates kind
  constexpr inline auto real64 = as<double>;

  /// @brief  Pre-defined type settings for std::int8_t
  /// @relates kind
  constexpr inline auto int8 = as<std::int8_t>;

  /// @brief Pre-defined type settings for std::int16_t
  /// @relates kind
  constexpr inline auto int16 = as<std::int16_t>;

  /// @brief Pre-defined type settings for std::int32_t
  /// @relates kind
  constexpr inline auto int32 = as<std::int32_t>;

  /// @brief Pre-defined type settings for std::int64_t
  /// @relates kind
  constexpr inline auto int64 = as<std::int64_t>;

  /// @brief Pre-defined type settings for std::uint8_t
  /// @relates kind
  constexpr inline auto uint8 = as<std::uint8_t>;

  /// @brief Pre-defined type settings for std::uint16_t
  /// @relates kind
  constexpr inline auto uint16 = as<std::uint16_t>;

  /// @brief Pre-defined type settings for std::uint32_t
  /// @relates kind
  constexpr inline auto uint32 = as<std::uint32_t>;

  /// @brief Pre-defined type settings for std::uint64_t
  /// @relates kind
  constexpr inline auto uint64 = as<std::uint64_t>;
}
