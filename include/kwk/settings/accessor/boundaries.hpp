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
    template<typename T> struct boundary_option;

    struct boundary_id : kumi::identifier<boundary_id>
    {
      template<typename T> constexpr auto operator=(boundary_option<T> const& cnst) const noexcept { return cnst; }

      friend constexpr auto to_str(boundary_id) { return kumi::str{"Boundary Conditions"}; }
    };

    template<typename T> struct boundary_option : T
    {
      using element_type = T;
      using type = boundary_option<T>;
      using identifier_type = boundary_id;
      using label_type = kumi::str;
      using T::operator();

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr label_type label() { return kumi::str{"Indexing"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           boundary_option const&) noexcept
      {
        return os << label << kumi::_::typer<T>();
      }
    };
  }

  //================================================================================================
  //! @ingroup  settings
  //! @brief    Indexing setting for kwk::table
  //================================================================================================
  inline constexpr __::boundary_id boundary{};

  template<typename T> constexpr __::boundary_option<T> bounds(T = {})
  {
    return {};
  }

  namespace __
  {
    struct clamp_t
    {
      template<kumi::concepts::product_type P, auto... shp>
      static constexpr auto operator()(P&& p, kwk::shape<shp...> const& sh) noexcept
      {
        return kumi::map(
          [&](auto&& t, auto&& s) {
            if constexpr (kumi::concepts::product_type<decltype(t)>) return clamp_t{}(KWK_FWD(t), KWK_FWD(s));
            else
            {
              auto tp = static_cast<std::ptrdiff_t>(t);
              auto sp = static_cast<std::ptrdiff_t>(s);
              return tp < 0 ? 0 : (tp > sp ? sp : tp);
            }
          },
          KWK_FWD(p), sh);
      }
    };

    struct tile_t
    {
      template<kumi::concepts::product_type P, auto... shp>
      static constexpr auto operator()(P&& p, kwk::shape<shp...> const& sh) noexcept
      {
        return kumi::map(
          [&](auto&& t, auto&& s) {
            if constexpr (kumi::concepts::product_type<decltype(t)>) return tile_t{}(KWK_FWD(t), KWK_FWD(s));
            else
            {
              auto tp = static_cast<std::ptrdiff_t>(t);
              auto sp = static_cast<std::ptrdiff_t>(s);
              auto r = tp % sp;

              return sp == 0 ? 0 : (r < 0 ? (r + sp) : r);
            }
          },
          KWK_FWD(p), sh);
      }
    };

    struct mirror_t
    {
      template<kumi::concepts::product_type P, auto... shp>
      static constexpr auto operator()(P&& p, kwk::shape<shp...> const& sh) noexcept
      {
        return kumi::map(
          [&](auto&& t, auto&& s) {
            if constexpr (kumi::concepts::product_type<decltype(t)>) return mirror_t{}(KWK_FWD(t), KWK_FWD(s));
            else
            {
              auto tp = static_cast<std::ptrdiff_t>(t);
              auto sp = static_cast<std::ptrdiff_t>(s);
              auto period = 2 * (sp - 1);
              auto x = tp % period;
              x = x < 0 ? x + period : x;
              return sp <= 1 ? 0 : (x < sp ? x : period - x);
            }
          },
          KWK_FWD(p), sh);
      }
    };
  }

  // Boundary condition
  inline constexpr auto clamp = bounds<kwk::__::clamp_t>();
  inline constexpr auto tile = bounds<kwk::__::tile_t>();
  inline constexpr auto mirror = bounds<kwk::__::mirror_t>();
}
