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
  template<typename T> struct kind_option;

  struct kind_id : kumi::identifier<kind_id>
  {
    template<typename T> constexpr auto operator=(kind_option<T> const& cnst) const noexcept { return cnst; }

    friend constexpr auto to_str(kind_id) { return kumi::str{"kind"}; }
  };

  inline constexpr kind_id kind = {};

  //====================================================================================================================
  /**
    @ingroup  settings
    @brief    Kind setting for KIWAKU containers

    @tparam   T Type to be used within the container
  **/
  //====================================================================================================================
  template<typename T> struct kind_option : kumi::identifier<kind_id>
  {
    /// The type to be used within the container
    using element_type = T;
    using type = kind_option<T>;
    using identifier_type = kind_id;

    constexpr auto operator()(kind_id) const { return *this; }

    static constexpr auto name() { return kumi::str{"kind"}; }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         kind_option const&) noexcept
    {
      return os << "kind : " << kumi::_::typer<T>();
    }
  };

  //====================================================================================================================
  /**
    @ingroup  settings
    @brief    Helper to create type settings for KIWAKU containers

    @tparam   T Type to be used within the container
    @relates  kind
  **/
  //====================================================================================================================
  template<typename T> constexpr kind_option<T> as()
  {
    return {};
  }

  template<typename T> constexpr kind_option<T> as(T)
  {
    return {};
  }

  /// @brief Pre-defined type settings for float
  /// @relates kind
  constexpr inline auto real32 = as<float>();

  /// @brief Pre-defined type settings for double
  /// @relates kind
  constexpr inline auto real64 = as<double>();

  /// @brief  Pre-defined type settings for std::int8_t
  /// @relates kind
  constexpr inline auto int8 = as<std::int8_t>();

  /// @brief Pre-defined type settings for std::int16_t
  /// @relates kind
  constexpr inline auto int16 = as<std::int16_t>();

  /// @brief Pre-defined type settings for std::int32_t
  /// @relates kind
  constexpr inline auto int32 = as<std::int32_t>();

  /// @brief Pre-defined type settings for std::int64_t
  /// @relates kind
  constexpr inline auto int64 = as<std::int64_t>();

  /// @brief Pre-defined type settings for std::uint8_t
  /// @relates kind
  constexpr inline auto uint8 = as<std::uint8_t>();

  /// @brief Pre-defined type settings for std::uint16_t
  /// @relates kind
  constexpr inline auto uint16 = as<std::uint16_t>();

  /// @brief Pre-defined type settings for std::uint32_t
  /// @relates kind
  constexpr inline auto uint32 = as<std::uint32_t>();

  /// @brief Pre-defined type settings for std::uint64_t
  /// @relates kind
  constexpr inline auto uint64 = as<std::uint64_t>();
}
