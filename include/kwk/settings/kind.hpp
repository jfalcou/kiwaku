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
    template<typename T> struct kind_option;

    struct kind_id : kumi::identifier<kind_id>
    {
      template<typename T> constexpr auto operator=(kind_option<T> const& cnst) const noexcept { return cnst; }

      friend constexpr auto to_str(kind_id) { return kumi::str{"Kind"}; }
    };

    template<typename T> struct kind_option
    {
      using element_type = T;
      using type = kind_option<T>;
      using identifier_type = kind_id;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Kind"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           kind_option const&) noexcept
      {
        return os << "Kind : " << kumi::_::typer<T>();
      }
    };

    template<typename T> constexpr bool operator==(kind_option<T>, kind_option<T>)
    {
      return true;
    };

    template<typename T, typename U> constexpr bool operator==(kind_option<T>, kind_option<U>)
    {
      return false;
    };
  }

  //====================================================================================================================
  /**
  @name Type settings helpers
  @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Identifier for the kind setting for containers

    Identifies the kind setting for KIWAKU containers, which specifies the type to be used within the container.
  **/
  //====================================================================================================================
  inline constexpr _::kind_id kind = {};

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief    Helper to create type settings for KIWAKU containers

    @tparam   T Type to be used within the container
    @see  kind
  **/
  //====================================================================================================================
  template<typename T> constexpr _::kind_option<T> as(T = {})
  {
    return {};
  }

  //====================================================================================================================
  /**
    @}
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @name Pre-defined type settings
    @brief Ready-to-use type settings for common types
    @{
  **/
  //====================================================================================================================

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for float
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto real32 = as<float>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for double
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto real64 = as<double>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief  Pre-defined type settings for std::int8_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto int8 = as<std::int8_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::int16_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto int16 = as<std::int16_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::int32_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto int32 = as<std::int32_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::int64_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto int64 = as<std::int64_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::uint8_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto uint8 = as<std::uint8_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::uint16_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto uint16 = as<std::uint16_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::uint32_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto uint32 = as<std::uint32_t>();

  //====================================================================================================================
  /**
    @ingroup  settings-kind
    @brief Pre-defined type settings for std::uint64_t
    @see kind
  **/
  //====================================================================================================================
  constexpr inline auto uint64 = as<std::uint64_t>();

  //====================================================================================================================
  /**
    @}
  **/
  //====================================================================================================================
}
