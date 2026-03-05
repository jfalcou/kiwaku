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
    template<kumi::concepts::record_type T> struct axis_option;

    struct axis_id : kumi::identifier<axis_id>
    {
      template<kumi::concepts::record_type R> constexpr auto operator=(R&& r) const noexcept
      {
        return axis_option<R>{KWK_FWD(r)};
      }

      friend constexpr auto to_str(axis_id) { return kumi::str{"Axis"}; }
    };

    template<kumi::concepts::record_type R> struct axis_option
    {
      using element_type = R;
      using type = axis_option<R>;
      using identifier_type = axis_id;

      R value;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Axis"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           axis_option const& a) noexcept
      {
        return os << "Axis: " << a.value;
      }
    };
  }

  inline constexpr _::axis_id axis{};
}
