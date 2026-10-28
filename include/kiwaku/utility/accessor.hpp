//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

#include <string_view>

#include <kiwaku/utility/accessor/boundaries.hpp>
#include <kiwaku/utility/accessor/coordinate.hpp>
#include <kiwaku/utility/accessor/interpolator.hpp>

namespace kwk
{
  // An accessor is a fibre : a chain of plain function objects that resolves a position to a value
  // read from storage. The stages are composed by piping; reading flows through them backwards
  // (continuation-passing) :
  //
  //   - the innermost piped stage is the resolver (the coordinate / index function, e.g. canonical)
  //     : it maps `(position, stride)` to an offset and is the leaf of the chain;
  //   - a boundary (clamp / tile / mirror) is a stage that transforms `(position, shape)` and then
  //     *passes the value down* to the rest of the chain;
  //   - an interpolator (none / nearest / floor / ceil / round / linear) holds a callback into the
  //     rest of the chain and *calls it back* for each sample it fans out, then blends the results.
  //
  // The stages are plain callables, so composing is just piping objects : `accessor{canonical}`,
  // `accessor{canonical} | clamp`, `accessor{canonical} | clamp | linear`. There is no "rule"
  // machinery and no per-axis scoping : a boundary or interpolator applies to the whole fibre, and
  // an absent boundary is simply *not piped*.
  template<typename... Stages> struct accessor_t;

  namespace __
  {
    template<typename T> inline constexpr bool always_false_v = false;

    template<typename T> struct is_accessor : std::false_type
    {
    };
    template<typename... T> struct is_accessor<accessor_t<T...>> : std::true_type
    {
    };

    template<typename T> inline constexpr bool is_accessor_v = is_accessor<T>::value;

    // Identifier of the accessor option (part of the uniform "field over itself" option protocol).
    struct accessor_id : kumi::identifier<accessor_id>
    {
      friend constexpr auto to_str(accessor_id) { return kumi::str {"Accessor"}; }
    };

    // Classification of a piped stage by its option identifier. The public strategy constants
    // (kwk::canonical, kwk::clamp, ..., kwk::linear) are all option wrappers carrying one of these.
    // The primary template matches any `T`; only a partial specialization *also* carrying the right
    // `identifier_type` yields true — so a stage without one (e.g. kumi::unit) is simply "false"
    // without a hard error.
    template<typename T, typename Id> struct identifier_check : std::false_type
    {
    };
    template<typename T, typename Id>
      requires (requires { typename T::identifier_type; })
    struct identifier_check<T, Id>
        : std::bool_constant<std::same_as<typename T::identifier_type, Id>>
    {
    };

    template<typename T> inline constexpr bool is_resolver_v     = identifier_check<T, coordinate_id>::value;
    template<typename T> inline constexpr bool is_boundary_v     = identifier_check<T, boundary_id>::value;
    template<typename T> inline constexpr bool is_interpolator_v = identifier_check<T, interpolator_id>::value;

    // The raw strategy of a stage : options wrap a strategy in `element_type`, unwrap it, otherwise
    // the stage is already the strategy.
    template<typename T>
    using stage_strategy_t =
        std::conditional_t<requires { typename T::element_type; }, typename T::element_type, T>;

    // Whether a stage's strategy blends (`linear`) or merely re-samples a position (`nearest`, ...).
    // Mirrors concepts/interpolator.hpp's `_is_interpolating` / `_selects` markers.
    template<typename T>
    inline constexpr bool stage_interpolates_v =
        requires { stage_strategy_t<T>::_is_interpolating; };

    template<typename T>
    inline constexpr bool stage_fractional_v =
        requires { stage_strategy_t<T>::_is_interpolating; } ||
        requires { stage_strategy_t<T>::_selects; };

    // The origin shift of a resolver (identity for canonical / 0-based, subtracts the base for
    // based_t). It runs once on the position *before* any boundary so a boundary sees already-shifted
    // physical indices (see based_t in coordinate.hpp).
    template<typename Sink, typename Position> constexpr auto shift_position(Position const& p)
    {
      if constexpr(requires { Sink::shift(p); }) return Sink::shift(p);
      else return std::remove_cvref_t<Position>(p);
    }

    // The resolver leaf : an index function maps `(position, stride)` to an offset (or a
    // self-describing value) and hands the storage offset to the read callback — the only place a
    // read meets the storage.
    template<typename Sink, typename Position, typename Stride, typename Read>
    constexpr decltype(auto) resolver_leaf(Sink const& sink, Position const& p, Stride const& sd,
                                           Read&& read)
    {
      auto r = sink(p, sd);
      if constexpr(std::is_integral_v<std::remove_cvref_t<decltype(r)>>)
        return read(static_cast<std::size_t>(r));
      else return (r);
    }

    // Interpolation fan-out : an interpolator calls the rest of the chain back per sample.
    //
    //   - selection strategies (nearest / floor / ceil / round) pick one integer grid position and
    //     call the rest with it once;
    //   - linear computes the 2^N surrounding corners and calls the rest with each, blending weights;
    //   - none (the default when nothing was piped) passes the (integral) position straight down.
    template<typename Strategy, typename Position, typename Stride, typename Inner>
    constexpr auto linear_blend(Position const& p, Stride const& sd, Inner&& inner)
    {
      auto const&                sample = Strategy::down(p, sd);
      auto const&                base   = kumi::get<0>(sample);
      auto const&                frac   = kumi::get<1>(sample);
      constexpr std::size_t      N      = kumi::size_v<std::remove_cvref_t<decltype(base)>>;

      // Seed the accumulator with the mask-0 corner so its type is that of `weight * sample` — a
      // floating-point promotion of an integral sample (e.g. a generator yielding `size_t`), exactly
      // what a weighted blend needs, and a per-fibre tuple for a product (SoA) sample — then fold the
      // remaining corners in.
      auto acc = Strategy::weight(frac, 0) * inner(Strategy::corner(base, 0));
      for(std::size_t mask = 1; mask < (std::size_t {1} << N); ++mask)
        acc = acc + Strategy::weight(frac, mask) * inner(Strategy::corner(base, mask));
      return acc;
    }

    template<typename Stage, typename Position, typename Shape, typename Stride, typename Inner>
    constexpr decltype(auto) interpolate(Stage const&, Position const& p, Shape const&,
                                         Stride const& sd, Inner&& inner)
    {
      using strategy = stage_strategy_t<std::remove_cvref_t<Stage>>;

      if constexpr(requires { strategy::_selects; }) return inner(strategy::select(p));
      else if constexpr(requires { strategy::_is_interpolating; })
        return linear_blend<strategy>(p, sd, KWK_FWD(inner));
      else return inner(p);
    }

    // Delegate a read to the stage at index J, then inward. The innermost stage (J == 0) is the
    // resolver leaf; every outer stage either passes its transformed position down (boundary) or
    // calls the rest back per sample (interpolator).
    template<std::size_t J, typename Stages, typename Position, typename Shape, typename Stride,
             typename Read>
    constexpr decltype(auto) run_stage(Stages const& stages, Position const& p, Shape const& sp,
                                       Stride const& sd, Read&& read)
    {
      if constexpr(J == 0) return resolver_leaf(kumi::get<0>(stages), p, sd, KWK_FWD(read));
      else
      {
        using stage_t = std::remove_cvref_t<decltype(kumi::get<J>(stages))>;
        auto inner    = [&](auto const& q) -> decltype(auto)
        { return run_stage<J - 1>(stages, q, sp, sd, read); };

        if constexpr(is_boundary_v<stage_t>) return inner(kumi::get<J>(stages)(p, sp));
        else if constexpr(is_interpolator_v<stage_t>)
          return interpolate(kumi::get<J>(stages), p, sp, sd, KWK_FWD(inner));
        else
          static_assert(__::always_false_v<stage_t>,
                        "kwk::accessor : an unknown stage was piped. A stage is either a boundary "
                        "(clamp/tile/mirror), an interpolator (none/nearest/linear/floor/ceil/round) "
                        "or the innermost resolver (canonical).");
      }
    }
  }

  //====================================================================================================================
  /*
    The accessor (fibre) : a chain of plain function objects, composed by piping, that resolves a
    position to a value read from storage.

        accessor_t{ canonical }                     // plain index function, no boundary / interpolation
        accessor_t{ canonical } | clamp             // boundary only
        accessor_t{ canonical } | linear            // interpolation only
        accessor_t{ canonical } | clamp | linear    // boundary feeds the interpolator's corners

    Reading is `acc(position, shape, stride, read)` : the accessor holds only its chain (no shape,
    stride or storage), the caller wires shape + stride + a storage-read callback `read(offset)` at
    read time — an accessor has no reference to the collection, so the collection provides that read.
  */
  //====================================================================================================================
  //====================================================================================================================
  // A named fibre : wraps a plain `accessor_t` (fibre chain) with an optional runtime label. Only
  // created by `accessor_t::named` — a plain, unnamed fibre carries no label storage so it stays an
  // empty type. A named fibre is still readable exactly like a plain one, and `.accessor()` yields
  // the underlying plain fibre (for embedding it in a collection alongside a runtime name).
  template<typename Accessor> struct named_fibre;

  template<typename... Stages> struct accessor_t
  {
    //==================================================================================================================
    // Uniform option protocol backbone (element_type / type / identifier_type / label_type ...).
    //==================================================================================================================
    using element_type    = accessor_t;
    using type            = accessor_t;
    using accessor_type   = accessor_t;
    using identifier_type = __::accessor_id;
    using label_type      = kumi::str;

    template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
    {
      using self_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                        accessor_t const,
                                        accessor_t>;
      return std::forward_like<Self>(static_cast<self_t&>(self));
    }

    template<typename Self> constexpr decltype(auto) accessor(this Self&& self) noexcept
    {
      using fwd_t = decltype(std::forward_like<Self>(std::declval<accessor_t>()));
      return static_cast<fwd_t>(self);
    }

    static constexpr label_type label() { return kumi::str {"Accessor"}; }

    // Whether any piped stage blends across samples (`linear`) or merely re-samples a fractional
    // position (`nearest`/`floor`/`ceil`/`round`). These drive the collection's fractional gate.
    static constexpr bool interpolates = (__::stage_interpolates_v<Stages> || ...);
    static constexpr bool fractional   = (__::stage_fractional_v<Stages> || ...);

    //==================================================================================================================
    // Construction and composition
    //==================================================================================================================
    constexpr explicit accessor_t() = default;

    // From a ready-made chain (used by CTAD on `accessor_t{ canonical }` and by the `|` operators).
    constexpr explicit accessor_t(kumi::tuple<Stages...> const& rs) : rules_(rs)
    {
    }

    template<typename... R>
    requires(!(sizeof...(R) == 1 &&
               (kumi::concepts::product_type<std::remove_reference_t<R>> && ...)))
    constexpr explicit accessor_t(R&&... rs) : rules_(kumi::make_tuple(std::forward<R>(rs)...))
    {
    }

    //==================================================================================================================
    // Runtime fibre name : a fibre may carry an (optional, runtime) label — built from any string
    // via `str::from` — used e.g. to name a variable backed by that fibre. The plain fibre keeps no
    // name storage (staying an empty type, so a storage-free generator table remains `sizeof == 1`);
    // naming it wraps it in a `named_fibre` that carries the runtime label. An empty name means
    // "unnamed".
    //==================================================================================================================
    // Naming returns a wrapper holding a copy of this fibre plus the runtime label.
    constexpr auto named(std::string_view n) const
    {
      return named_fibre<accessor_t> {
          *this, kumi::str::from(n.data(), static_cast<kumi::str::size_type>(n.size()))};
    }

    // The unnamed fibre's runtime name is always the empty string.
    constexpr kumi::str const& name() const noexcept
    {
      static constexpr kumi::str empty {""};
      return empty;
    }

    // Pipe a stage (a boundary or an interpolator) onto the chain. The last piped stage is the
    // outermost : it runs first on the position and either passes down or calls the rest back.
    template<typename Self, typename Stage>
    requires(!__::is_accessor_v<std::remove_cvref_t<Stage>>)
    constexpr auto operator|(this Self&& self, Stage&& stage)
    {
      using stage_t = std::remove_cvref_t<Stage>;
      static_assert(__::is_boundary_v<stage_t> || __::is_interpolator_v<stage_t>,
                    "kwk::accessor : only a boundary (clamp/tile/mirror) or an interpolator "
                    "(none/nearest/linear/floor/ceil/round) can be piped onto a fibre. The resolver "
                    "(canonical) is the innermost stage, piped at construction time.");
      return accessor_t<Stages..., stage_t> {
          kumi::cat(KWK_FWD(self).rules_, kumi::tuple<stage_t> {std::forward<Stage>(stage)})};
    }

    //==================================================================================================================
    /*
      Read : position -> value, given shape, stride and a storage read callback.

      The resolver's origin shift (base index) runs first, so boundaries see physical indices; then
      the chain runs from the outermost piped stage inward : a boundary transforms and passes the
      position down, an interpolator calls the rest back per sample, and the resolver leaf maps the
      position to a storage offset which the `read` callback resolves to a value.
    */
    //==================================================================================================================
    template<typename Self, typename Position, typename Shape, typename Stride, typename Read>
    constexpr decltype(auto)
    operator()(this Self&& self, Position&& p, Shape const& sp, Stride const& sd, Read&& read)
    {
      using sink_t = std::remove_cvref_t<decltype(kumi::get<0>(self.rules_))>;
      auto p0      = __::shift_position<sink_t>(KWK_FWD(p));
      return __::run_stage<sizeof...(Stages) - 1>(self.rules_, p0, sp, sd, KWK_FWD(read));
    }

    kumi::tuple<Stages...> rules_ {};
  };

  // Deduction guide : `accessor_t{ canonical }`, `accessor_t{ canonical, clamp }` deduce the piped
  // stage types from the arguments. (`accessor_t{ some_kumi_tuple }` routes to the tuple-constructor
  // guide below, if such an explicit CTAD is ever performed.)
  template<typename... R> accessor_t(R&&...) -> accessor_t<std::remove_cvref_t<R>...>;

  namespace __
  {
    // Build a fibre from a ready-made chain tuple `(resolver, boundary?, interpolator?)`, unwrapping
    // each element into its own stage. Used by the builder to assemble the final accessor when some
    // stages are absent (rather than piped as a "no-boundary" marker).
    template<typename Tuple, std::size_t... I>
    constexpr auto accessor_from_tuple_impl(Tuple const& t, std::index_sequence<I...>)
    {
      return accessor_t<kumi::element_t<I, Tuple>...> {kumi::get<I>(t)...};
    }

    template<typename Tuple> constexpr auto accessor_from_tuple(Tuple const& t)
    {
      return accessor_from_tuple_impl(t, std::make_index_sequence<kumi::size_v<Tuple>> {});
    }
  }

  //! Alias for the accessor type (natural name of the property).
  template<typename... Stages> using accessor = accessor_t<Stages...>;

  // Alias matching the vocabulary of the scheme: an accessor reads one fibre of a collection.
  template<typename... Stages> using fibre = accessor_t<Stages...>;

  //====================================================================================================================
  // A named fibre wraps a plain `accessor_t` plus a runtime label. It reads exactly like the plain
  // fibre (same 4-argument entry point), reports its label through `name()`, and hands out the
  // underlying plain fibre through `accessor()` — so it can be embedded in a collection carrying a
  // runtime name while the plain (unnamed) fibre itself stays an empty type.
  //====================================================================================================================
  template<typename Accessor> struct named_fibre
  {
    Accessor  accessor_ {};
    kumi::str name_    {};

    // The runtime label (the empty string when it was never set).
    constexpr kumi::str const& name() const noexcept { return name_; }

    // Read : transparently delegate to the underlying plain fibre.
    template<typename Position, typename Shape, typename Stride, typename Read>
    constexpr decltype(auto) operator()(Position&& p, Shape const& sp, Stride const& sd,
                                        Read&& read) const
    {
      return accessor_(KWK_FWD(p), sp, sd, KWK_FWD(read));
    }

    // The underlying plain (unnamed) fibre.
    constexpr decltype(auto) accessor() const noexcept { return accessor_; }
  };
}
