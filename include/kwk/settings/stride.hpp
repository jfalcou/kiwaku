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
    template<kumi::concepts::projection_map T> struct stride_option;

    struct stride_id : kumi::identifier<stride_id>
    {
      template<kumi::concepts::projection_map I> constexpr auto operator=(I i) const noexcept
      {
        return stride_option<I>{i};
      }

      friend constexpr auto to_str(stride_id) { return kumi::str{"Stride"}; }
    };

    template<kumi::concepts::projection_map T> struct stride_option
    {
      using element_type = T;
      using type = stride_option<T>;
      using identifier_type = stride_id;

      T value;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Stride"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           stride_option const& s) noexcept
      {
        return os << "Stride : " << s.value;
      }
    };
  }

  inline constexpr _::stride_id stride{};
}
