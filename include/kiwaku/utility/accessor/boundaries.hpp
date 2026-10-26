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
      template<typename T> constexpr auto operator=(boundary_option<T> const& cnst) const noexcept
      {
        return cnst;
      }

      friend constexpr auto to_str(boundary_id) { return kumi::str {"Boundary Conditions"}; }
    };

    template<typename T> struct boundary_option : T
    {
      using element_type    = T;
      using type            = boundary_option<T>;
      using identifier_type = boundary_id;
      using label_type      = kumi::str;
      using T::operator();

      template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
      {
        using boundary_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                              boundary_option const,
                                              boundary_option>;
        return std::forward_like<Self>(static_cast<boundary_t&>(self));
      }

      static constexpr label_type label() { return kumi::str {"Boundary Conditions"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           boundary_option const&) noexcept
      {
        return os << label << kumi::_::typer<T>();
      }
    };
  }

  template<typename T> constexpr __::boundary_option<T> bounds(T = {}) { return {}; }

  namespace __
  {
    struct clamp_t
    {
      template<kumi::concepts::product_type P, auto... shp>
      static constexpr auto operator()(P&& p, kwk::shape<shp...> const& sh) noexcept
      {
        return kumi::map(
            [ & ](auto&& t, auto&& s)
            {
              if constexpr(kumi::concepts::product_type<decltype(t)>)
                return clamp_t {}(KWK_FWD(t), KWK_FWD(s));
              else
              {
                auto tp = static_cast<std::ptrdiff_t>(t);
                auto sp = static_cast<std::ptrdiff_t>(s);
                // Valid indices span [0, sp-1] : clamp into that range.
                return tp < 0 ? 0 : (tp >= sp ? sp - 1 : tp);
              }
            },
            KWK_FWD(p),
            sh);
      }
    };

    struct tile_t
    {
      template<kumi::concepts::product_type P, auto... shp>
      static constexpr auto operator()(P&& p, kwk::shape<shp...> const& sh) noexcept
      {
        return kumi::map(
            [ & ](auto&& t, auto&& s)
            {
              if constexpr(kumi::concepts::product_type<decltype(t)>)
                return tile_t {}(KWK_FWD(t), KWK_FWD(s));
              else
              {
                auto tp = static_cast<std::ptrdiff_t>(t);
                auto sp = static_cast<std::ptrdiff_t>(s);
                // Guard zero-extent before any modulo (tp % 0 is UB), then fold into [0, sp-1].
                if(sp == 0) return std::ptrdiff_t {0};
                auto r = tp % sp;
                return r < 0 ? (r + sp) : r;
              }
            },
            KWK_FWD(p),
            sh);
      }
    };

    struct mirror_t
    {
      template<kumi::concepts::product_type P, auto... shp>
      static constexpr auto operator()(P&& p, kwk::shape<shp...> const& sh) noexcept
      {
        return kumi::map(
            [ & ](auto&& t, auto&& s)
            {
              if constexpr(kumi::concepts::product_type<decltype(t)>)
                return mirror_t {}(KWK_FWD(t), KWK_FWD(s));
              else
              {
                auto tp = static_cast<std::ptrdiff_t>(t);
                auto sp = static_cast<std::ptrdiff_t>(s);
                // sp==1 mirrors onto the single index 0; sp==0 is an empty dim. Guard both before
                // computing the period (period=2*(sp-1) would be 0 for sp==1 -> tp % 0 is UB).
                if(sp <= 1) return std::ptrdiff_t {0};
                auto period = 2 * (sp - 1);
                auto x      = tp % period;
                x           = x < 0 ? x + period : x;
                return x < sp ? x : period - x;
              }
            },
            KWK_FWD(p),
            sh);
      }
    };
  }

  // Boundary condition
  inline constexpr auto clamp  = bounds<kwk::__::clamp_t>();
  inline constexpr auto tile   = bounds<kwk::__::tile_t>();
  inline constexpr auto mirror = bounds<kwk::__::mirror_t>();
}
