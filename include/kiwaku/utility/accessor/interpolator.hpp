//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <cmath>

namespace kwk
{
  namespace __
  {
    template<typename T> struct interpolator_option;

    struct interpolator_id : kumi::identifier<interpolator_id>
    {
      template<typename T> constexpr auto operator=(interpolator_option<T> const& cnst) const noexcept { return cnst; }

      friend constexpr auto to_str(interpolator_id) { return kumi::str{"Interpolator"}; }
    };

    template<typename T> struct interpolator_option
    {
      using element_type = T;
      using type = interpolator_option<T>;
      using identifier_type = interpolator_id;
      using label_type = kumi::str;

      constexpr auto operator()(identifier_type) const { return *this; }

      static constexpr auto name() { return kumi::str{"Interpolator"}; }

      static constexpr label_type label() { return kumi::str{"Interpolator"}; }

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
    //==================================================================================================================
    // Interpolator strategies (bidirectional pipeline).
    //
    // Each strategy provides two stages that the accessor/collection wire together:
    //   - `down(position, shape, stride)` : the "solve layout" stage. It maps a (possibly
    //     fractional) position onto a *sample request* describing which flat storage elements are
    //     needed and how they combine.
    //   - `up(sample, shape, stride, read)` : the "accessor up" stage. `read(i)` retrieves the raw
    //     value at flat scalar index `i`; `up` combines the sampled values into the final one.
    //
    // `none` and `nearest` are pure selections (the up stage reads exactly one element), which also
    // make them the correct default for exact grid sampling. `linear` gathers all 2^N surrounding
    // corners and blends them (multi-linear interpolation in the flattened grid).
    //==================================================================================================================

    // Exact grid sampling : a sample is simply a flat index.
    struct none {};

    struct nearest
    {
      static constexpr bool _selects = true;

      // Selection: round each flat coordinate to the nearest integer grid point. The accessor then
      // flattens the position through its coordinate stage, so `nearest` behaves like `round` while
      // remaining a value-free strategy (no coordinate dependency).
      template<kumi::concepts::product_type Position>
      static constexpr auto select(Position&& p)
      {
        return kumi::map(
          [](auto&& x) {
            if constexpr (kumi::concepts::product_type<decltype(x)>)
              return nearest{}.select(KWK_FWD(x));
            else
              return static_cast<std::ptrdiff_t>(std::round(x));
          },
          KWK_FWD(p));
      }
    };

    // Multi-linear interpolation. A sample is `kumi::tuple{ base, frac }` where `base` and `frac`
    // are flat tuples (one element per flattened dimension): base_i = floor(p_i), frac_i = p_i -
    // base_i in [0,1).
    struct linear
    {
      static constexpr bool _is_interpolating = true;

      // Downstage: split a (possibly nested) position into per-dim base + fraction.
      template<kumi::concepts::product_type Position, typename Stride>
      static constexpr auto down(Position&& p, Stride const&)
      {
        auto const flat = kumi::flatten_all(KWK_FWD(p));
        // base_i = floor(p_i), frac_i = p_i - floor(p_i) in [0,1). Truncation toward zero would give
        // a wrong base and a *negative* frac for a negative position (extrapolation weight); floor
        // keeps frac in [0,1) regardless of sign.
        auto base = kumi::map( [](auto&& x) { return static_cast<std::ptrdiff_t>(std::floor(x)); }, flat );
        auto frac = kumi::map( [](auto&& x)
                               { return static_cast<double>(KWK_FWD(x))
                                      - std::floor(static_cast<double>(KWK_FWD(x))); }
                             , flat );
        return kumi::tuple{ base, frac };
      }

      // Flat index of a base/corner tuple : the dot product of the base tuple with the flattenable
      // stride (a kumi inner_product over equal-sized tuples, no index unwrapping).
      template<typename Index, typename Stride>
      static constexpr std::ptrdiff_t flat(Index const& b, Stride const& sd)
      {
        auto const s = kumi::flatten_all(sd);
        return kumi::inner_product(b, s, std::ptrdiff_t{0});
      }

      // Weight of corner `mask`: per-dim frac_i where the bit is set, (1-frac_i) otherwise.
      template<typename Frac, std::size_t... I>
      static constexpr double weight_impl(Frac const& f, std::size_t mask, std::index_sequence<I...>)
      {
        return ( ( mask & (std::size_t{1} << I)
                    ? static_cast<double>(kumi::get<I>(f))
                    : (1.0 - static_cast<double>(kumi::get<I>(f))) ) * ... );
      }
      template<typename Frac>
      static constexpr double weight(Frac const& f, std::size_t mask)
      {
        return weight_impl(f, mask, std::make_index_sequence<kumi::size_v<Frac>>{});
      }

      // base_i + (bit i of mask is 1 ? 1 : 0)  ->  the "high side" base_i+1 when the bit is set.
      // (weight is the matching frac_i when the bit is set, (1-frac_i) when clear.)
      template<typename Base, std::size_t... I>
      static constexpr auto corner_impl(Base const& base, std::size_t mask, std::index_sequence<I...>)
      {
        return kumi::tuple{ ( kumi::get<I>(base) + ( (mask & (std::size_t{1} << I)) ? 1 : 0 ) )... };
      }
      template<typename Base>
      static constexpr auto corner(Base const& base, std::size_t mask)
      {
        return corner_impl(base, mask, std::make_index_sequence<kumi::size_v<Base>>{});
      }

      // Upstage: read every corner and blend. `read(i)` returns the raw value at flat index `i`.
      template<typename Sample, typename Shape, typename Stride, typename Read>
      static constexpr decltype(auto) up(Sample&& sample, Shape const&, Stride const& sd, Read&& read)
      {
        auto const& base = kumi::get<0>(sample);
        auto const& frac = kumi::get<1>(sample);
        constexpr std::size_t N = kumi::size_v<decltype(base)>;

        double acc = 0.0;
        for (std::size_t mask = 0; mask < (std::size_t{1} << N); ++mask)
        {
          auto w = weight(frac, mask);
          auto idx = flat(corner(base, mask), sd);
          acc += w * static_cast<double>(read(static_cast<std::size_t>(idx)));
        }
        return acc;
      }
    };

    struct floor
    {
      static constexpr bool _selects = true;

      // Selection: round each flat coordinate down to the nearest integer grid point (toward -inf).
      // This mirrors `nearest::select` but with floor rounding semantics, so a `floor` strategy is a
      // value-free exact sampler reachable through the accessor's select->down->up path.
      template<kumi::concepts::product_type Position>
      static constexpr auto select(Position&& p)
      {
        return kumi::map(
          [](auto&& elt) {
            if constexpr (kumi::concepts::product_type<decltype(elt)>) return floor::select(KWK_FWD(elt));
            else return static_cast<std::ptrdiff_t>(std::floor(KWK_FWD(elt)));
          },
          KWK_FWD(p));
      }
    };

    struct ceil
    {
      static constexpr bool _selects = true;

      // Selection: round each flat coordinate up to the nearest integer grid point (toward +inf).
      template<kumi::concepts::product_type Position>
      static constexpr auto select(Position&& p)
      {
        return kumi::map(
          [](auto&& elt) {
            if constexpr (kumi::concepts::product_type<decltype(elt)>) return ceil{}.select(KWK_FWD(elt));
            else return static_cast<std::ptrdiff_t>(std::ceil(KWK_FWD(elt)));
          },
          KWK_FWD(p));
      }
    };

    struct round
    {
      static constexpr bool _selects = true;

      // Selection: round each flat coordinate to the nearest integer grid point (half away from zero).
      template<kumi::concepts::product_type Position>
      static constexpr auto select(Position&& p)
      {
        return kumi::map(
          [](auto&& elt) {
            if constexpr (kumi::concepts::product_type<decltype(elt)>) return round{}.select(KWK_FWD(elt));
            else return static_cast<std::ptrdiff_t>(std::round(KWK_FWD(elt)));
          },
          KWK_FWD(p));
      }
    };
  }

  // Interpolation strategies
  inline constexpr auto floor = interpolation<kwk::__::floor>();
  inline constexpr auto ceil = interpolation<kwk::__::ceil>();
  inline constexpr auto round = interpolation<kwk::__::round>();
  inline constexpr auto nearest = interpolation<kwk::__::nearest>();
  inline constexpr auto linear = interpolation<kwk::__::linear>();
}
