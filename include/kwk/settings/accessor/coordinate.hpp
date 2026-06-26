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
  namespace __
  {
    template<typename T> struct coordinate_option;

    struct coordinate_id : kumi::identifier<coordinate_id>
    {
      template<typename T> constexpr auto operator=(coordinate_option<T> const& cnst) const noexcept { return cnst; }

      friend constexpr auto to_str(coordinate_id) { return kumi::str{"Coordinate"}; }
    };

    template<typename T> struct coordinate_option : T
    {
      using element_type = T;
      using type = coordinate_option<T>;
      using identifier_type = coordinate_id;
      using label_type = kumi::str;
      using T::operator();

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr label_type label() { return kumi::str{"Coordinate"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           coordinate_option const&) noexcept
      {
        return os << label << kumi::_::typer<T>();
      }
    };
  }

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Indexing setting for kwk::table
  //================================================================================================
  inline constexpr __::coordinate_id coordinate{};

  template<typename T> constexpr __::coordinate_option<T> coordinator(T = {})
  {
    return {};
  }

  namespace __
  {
    struct canonical_t
    {
      template<kumi::concepts::product_type P, auto... std>
      static constexpr auto operator()(P&& p, kwk::stride<std...> const& sd) noexcept
      {
        return kumi::inner_product(KWK_FWD(p), sd, 0);
      }
    };

    struct morton_t
    {
    };

    struct hilbert_t
    {
    };
  }

  // Basic strategies
  inline constexpr auto canonical = coordinator<kwk::__::canonical_t>();

  // Space filling curves
  // inline constexpr auto morton    = coordinator<morton_>();
  // inline constexpr auto hilbert   = coordinator<hilbert_>();
}
