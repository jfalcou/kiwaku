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
  // @brief Helper to convert characters to a proper integer in base10
  template<char... c> constexpr auto b10()
  {
    std::uint64_t value {0};
    ((value = value * 10 + (c - '0')), ...);
    return value;
  }

  // @brief Helper clamping a compile time integer to the smaller integer type that can contain it
  template<auto Value> constexpr auto clamp()
  {
    if constexpr(Value < 0)
    {
      if constexpr(Value >= -128) return static_cast<std::int8_t>(Value);
      else if constexpr(Value >= -32768) return static_cast<std::int16_t>(Value);
      else if constexpr(Value >= -2147483648LL) return static_cast<std::int32_t>(Value);
      else return Value;
    }
    else
    {
      if constexpr(Value <= 255) return static_cast<std::uint8_t>(Value);
      else if constexpr(Value <= 65535) return static_cast<std::uint16_t>(Value);
      else if constexpr(Value <= 4294967295ULL) return static_cast<std::uint32_t>(Value);
      else return Value;
    }
  }
}
