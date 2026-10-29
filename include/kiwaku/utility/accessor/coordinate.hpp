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
      template<typename T> constexpr auto operator=(coordinate_option<T> const& cnst) const noexcept
      {
        return cnst;
      }

      friend constexpr auto to_str(coordinate_id) { return kumi::str {"Coordinate"}; }
    };

    template<typename T> struct coordinate_option : T
    {
      using element_type    = T;
      using type            = coordinate_option<T>;
      using identifier_type = coordinate_id;
      using label_type      = kumi::str;
      using T::operator();

      template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
      {
        using coordinate_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                                coordinate_option const,
                                                coordinate_option>;
        return std::forward_like<Self>(static_cast<coordinate_t&>(self));
      }

      static constexpr label_type label() { return kumi::str {"Coordinate"}; }

      template<typename CharT, typename Traits>
      friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                           coordinate_option const&) noexcept
      {
        return os << label << kumi::_::typer<T>();
      }
    };
  }

  template<typename T> constexpr __::coordinate_option<T> coordinator(T = {}) { return {}; }

  namespace __
  {
    struct canonical_t
    {
      // Identity pre-stage : the canonical (0-based) coordinate does not shift the origin.
      static constexpr auto shift(auto&& p) { return KWK_FWD(p); }

      // Map a (possibly tiled/nested) position onto a flat element offset.
      //
      // Both the position and the stride are flattened before the inner product so that nested
      // (tiled) shapes/positions are handled uniformly: a `shape{3, shape{2,2}}` position
      // `(i, (ti,tj))` becomes the flat sum `i*sd_outer + ti*sd_inner0 + tj*sd_inner1`.
      template<kumi::concepts::product_type P, auto... std>
      static constexpr auto operator()(P&& p, kwk::stride<std...> const& sd) noexcept
      {
        return kumi::inner_product(kumi::flatten_all(KWK_FWD(p)), kumi::flatten_all(sd), 0);
      }
    };

    // A coordinate strategy whose origin is shifted so that index `Base` maps to element offset 0.
    // It is the mechanism behind the `base_index` setting (`c_index` = 0-based / `fortran_index` =
    // 1-based) : every flat coordinate of the position is reduced by `Base`, which shifts the
    // *origin* of the index space without ever altering the stored byte offsets.
    //
    // The base is *folded into the offset* : it is never added as a separate addressing layer. The
    // origin shift is expressed as the resolver's own `shift` pre-stage, which feeds the flattened
    // (physical, 0-based) coordinate into the plain offset inner-product below — the base
    // contributes exactly the constant scalar offset `-Base * sum(flatten(stride))` over the
    // flattened strides, with no `based` wrapper type. Keeping the shift *before* the boundary
    // stages matters : a boundary runs on already-shifted physical indices, so an in-range logical
    // index is never pushed out of bounds or doubly corrected. `Base == 0` degenerates to
    // `canonical_t`'s identity, which is why `c_index` and the (unset) default coincide.
    template<std::size_t Base> struct based_coord_t
    {
      static constexpr auto shift(auto&& p)
      {
        return kumi::map([](auto x) { return x - static_cast<std::ptrdiff_t>(Base); },
                         kumi::flatten_all(KWK_FWD(p)));
      }

      // The resolved offset is the plain flatten inner-product of the (already base-shifted)
      // physical position — identical to `canonical_t`, because the base has already been folded
      // into the position by `shift` before any boundary or interpolator stage sees it.
      template<kumi::concepts::product_type P, auto... std>
      static constexpr auto operator()(P&& p, kwk::stride<std...> const& sd) noexcept
      {
        return kumi::inner_product(kumi::flatten_all(KWK_FWD(p)), kumi::flatten_all(sd), 0);
      }
    };

    // Coordinate strategy guard : whether `T` is a coordinate strategy (has an address-resolving
    // `operator()`). Used to detect a wrapped/base-indexed coordinate inside the builder.
    template<typename T>
    inline constexpr bool is_coordinate_strategy_v =
        requires(T const& t) { t(kumi::tuple<std::ptrdiff_t> {}, kwk::stride<1, 1> {}); };

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
