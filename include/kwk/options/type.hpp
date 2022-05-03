//==================================================================================================
/**
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: MIT
**/
//==================================================================================================
#pragma once

#include <kwk/detail/raberu.hpp>
#include <cstdint>

namespace kwk
{
#if !defined(KWK_USE_DOXYGEN)
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

    // Extent from of_size(...)
    template<typename T> constexpr auto operator=(info<T> const& s) const noexcept
    {
      return s;
    }

    // Display
    template<typename T> std::ostream& display(std::ostream& os, info<T> const& s) const
    {
      return os << "Type: " << rbr::detail::type_name<T>();
    }
  };
#endif

  template<typename T> constexpr inline type_::info<T> type{};

  constexpr inline type_::info<float>         single_{};
  constexpr inline type_::info<double>        double_{};
  constexpr inline type_::info<std::int8_t>   int8_  {};
  constexpr inline type_::info<std::int16_t>  int16_ {};
  constexpr inline type_::info<std::int32_t>  int32_ {};
  constexpr inline type_::info<std::int64_t>  int64_ {};
  constexpr inline type_::info<std::uint8_t>  uint8_ {};
  constexpr inline type_::info<std::uint16_t> uint16_{};
  constexpr inline type_::info<std::uint32_t> uint32_{};
  constexpr inline type_::info<std::uint64_t> uint64_{};
}
