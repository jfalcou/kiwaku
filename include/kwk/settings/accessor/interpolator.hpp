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
    template<typename T> struct interpolator_option;

    struct interpolator_id : kumi::identifier<interpolator_id>
    {
      template<typename T> constexpr auto operator=(interpolator_option<T> const& cnst) const noexcept { return cnst; }

      friend constexpr auto to_str(interpolator_id) { return kumi::str{"Interpolator "}; }
    };

    template<typename T> struct interpolator_option
    {
      using element_type = T;
      using type = interpolator_option<T>;
      using identifier_type = interpolator_id;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Interpolator"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           interpolator_option const&) noexcept
      {
        return os << "Interpolator: " << kumi::_::typer<T>();
      }
    };
  }

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Indexing setting for kwk::table
  //================================================================================================
  inline constexpr __::interpolator_id interpolator{};

  template<typename T> constexpr __::interpolator_option<T> interpolation(T = {})
  {
    return {};
  }

  namespace __
  {
    struct floor
    {
      template<kumi::concepts::product_type Position> static constexpr auto operator()(Position&& p)
      {
        return kumi::map(
          [](auto&& elt) {
            if constexpr (kumi::concepts::product_type<decltype(elt)>) return kwk::__::floor{}(KWK_FWD(elt));
            else return static_cast<std::ptrdiff_t>(KWK_FWD(elt));
          },
          KWK_FWD(p));
      }
    };

    struct ceil
    {
      template<kumi::concepts::product_type Position> static constexpr auto operator()(Position&& p)
      {
        return kumi::map(
          [](auto&& elt) {
            if constexpr (kumi::concepts::product_type<decltype(elt)>) return kwk::__::ceil{}(KWK_FWD(elt));
            else return static_cast<std::ptrdiff_t>(KWK_FWD(elt)) + 1;
          },
          KWK_FWD(p));
      }
    };

    struct round
    {
      template<kumi::concepts::product_type Position> static constexpr auto operator()(Position&& p)
      {
        return kumi::map(
          [](auto&& elt) {
            if constexpr (kumi::concepts::product_type<decltype(elt)>) return kwk::__::round{}(KWK_FWD(elt));
            else return static_cast<std::ptrdiff_t>(KWK_FWD(elt) + 0.5);
          },
          KWK_FWD(p));
      }
    };

    // struct linear_
    //{
    //   template<kumi::concepts::product_type Position>
    //   static constexpr auto operator()(Position && p) noexcept
    //   {

    //  }
    //};

    // struct affine_
    //{
    //   static constexpr auto interpolate() noexcept {}
    // };
  }

  // Interpolation strategies
  inline constexpr auto floor = interpolation<kwk::__::floor>();
  inline constexpr auto ceil = interpolation<kwk::__::ceil>();
  inline constexpr auto round = interpolation<kwk::__::round>();
  // inline constexpr auto linear = interpolation<linear_>();
  // inline constexpr auto affine = interpolation<affine_>();
}
