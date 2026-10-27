//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/utility/accessor/boundaries.hpp>
#include <kiwaku/utility/accessor/coordinate.hpp>
#include <kiwaku/utility/accessor/interpolator.hpp>
#include <kiwaku/concepts/interpolator.hpp>

namespace kwk
{
  namespace __
  {
    // Identifier of the accessor option (part of the uniform "field over itself" option protocol).
    struct accessor_id : kumi::identifier<accessor_id>
    {
      friend constexpr auto to_str(accessor_id) { return kumi::str {"Accessor"}; }
    };

    //==================================================================================================================
    /*
      Down-walk across the interpolator pack (left-to-right).

      The first interpolating stage (which exposes a `down`, e.g. `linear`) produces the sample and
      stops the walk. A selection stage (`nearest`/`floor`/`ceil`/`round`) rounds the position and
      continues; an inert stage (`none`) passes it through. The base case flattens the (possibly
      rounded) position through the pipeline's coordinate stage.
    */
    //==================================================================================================================
    template<typename... Is> struct accessor_down_walk;

    template<> struct accessor_down_walk<>
    {
      template<typename Shape, typename Stride, typename Callback, typename Position>
      static constexpr decltype(auto) run(Position&& p, Shape const&, Stride const&, Callback&& cb)
      {
        return KWK_FWD(cb)(KWK_FWD(p));
      }
    };

    template<typename I0, typename... Is> struct accessor_down_walk<I0, Is...>
    {
      template<typename Shape, typename Stride, typename Callback, typename Position>
      static constexpr decltype(auto)
      run(Position&& p, Shape const& sp, Stride const& sd, Callback&& cb)
      {
        if constexpr(kwk::__::has_down_up_v<I0>)
        {
          return I0::down(KWK_FWD(p), KWK_FWD(sd));
        }
        else if constexpr(kwk::__::fractional_capable_v<I0>)
        {
          return accessor_down_walk<Is...>::run(I0::select(KWK_FWD(p)), sp, sd, KWK_FWD(cb));
        }
        else
        {
          return accessor_down_walk<Is...>::run(KWK_FWD(p), sp, sd, KWK_FWD(cb));
        }
      }
    };

    //==================================================================================================================
    /*
      Up-walk across the interpolator pack (left-to-right, mirroring the down walk).

      Locates the first interpolating stage and applies its `up`; the base case treats the sample
      itself as the flat index.
    */
    //==================================================================================================================
    template<typename... Is> struct accessor_up_walk;

    template<> struct accessor_up_walk<>
    {
      template<typename Shape, typename Stride, typename Read, typename Sample>
      static constexpr decltype(auto) run(Sample&& sample, Shape const&, Stride const&, Read&& read)
      {
        return KWK_FWD(read)(static_cast<std::size_t>(sample));
      }
    };

    template<typename I0, typename... Is> struct accessor_up_walk<I0, Is...>
    {
      template<typename Shape, typename Stride, typename Read, typename Sample>
      static constexpr decltype(auto)
      run(Sample&& sample, Shape const& sp, Stride const& sd, Read&& read)
      {
        if constexpr(kwk::__::has_down_up_v<I0>)
        {
          if constexpr(requires { I0::up(sample, sp, sd, read); })
            return I0::up(KWK_FWD(sample), sp, sd, KWK_FWD(read));
          else return KWK_FWD(read)(static_cast<std::size_t>(sample));
        }
        else
        {
          return accessor_up_walk<Is...>::run(KWK_FWD(sample), sp, sd, KWK_FWD(read));
        }
      }
    };
  }

  //====================================================================================================================
  /*
    The accessor is the bidirectional resolve pipeline between a position in index space and a value
    in storage space:

        solve layout -> accessor_down(indices, shape, stride) -> read(storage, index)
                        -> accessor_up(raw(s))            -> final value

    The *down* stage folds together the optional `Boundaries` (clamp/tile/mirror re-map of the
    index) and the `Coordinate` strategy (`canonical`, a space-filling curve, ...) that turns the
    index into a flat storage index. The *up* stage applies the `Interpolator`
    (none/nearest/linear/...) to the raw value(s) read out of storage. Absent stages contribute the
    identity transform.

    The accessor is a variadic pipeline of *stages*, listed positionally:

      - stage 0 : the **coordinate** strategy (`canonical`, `based_t<Base, Inner>`, ...) that maps a
        position onto a flat storage index (and, optionally, shifts the origin via `shift`).
      - stage 1 : the **boundaries** strategy (`clamp`/`tile`/`mirror`, or `kumi::unit` when absent)
        re-maps an out-of-range position back into the legal region.
      - stages 2+ : the **interpolator** stages (`none`/`nearest`/`linear`/`floor`/`ceil`/`round`)
        that turn a boundary-mapped position into a sample request (down) and blend the raw read(s)
        back into a value (up).

    The coordinate and boundary stages form the fixed addressing prefix. The interpolator stages are
    a zero-or-more pack. Because interpolators consume a *position* in down and a *sample* in up,
    and their sample types are strategy-specific, at most one interpolating stage can be engaged per
    pipeline : the first stage that exposes a `down` (an interpolating strategy such as `linear`)
    produces the sample and shadows any later interpolating stages. Selection strategies
    (`nearest`/`floor`/`ceil`/`round`) and inert stages (`none`) merely round / pass the position
    through. `accessor | accessor` concatenates two pipelines so a further stage set can be appended
    on the right.

    The accessor carries no storage of its own: it is a pure topology -> value pipeline. The raw
    storage is owned by the container's `properties` object, which feeds the accessor a `read(flat
    index) -> raw value` callback. This keeps the accessor free of any mixing of concerns between
    addressing (coordinate/boundary/interpolator) and memory (blob/scheme).
  */
  //====================================================================================================================
  template<typename Coordinate, typename Boundaries, typename... Interpolators>
  struct accessor_t
      : private Coordinate
      , private Boundaries
      , private Interpolators...
  {
    //! Whether the pipeline carries an interpolating (non-selection) stage that needs fractional
    //! positions (e.g. `linear`). Selection strategies (`none`/`nearest`) exact-sample.
    static constexpr bool interpolates = (kwk::__::has_down_up_v<Interpolators> || ...);

    //! Whether the pipeline can consume a fractional position: either it blends (interpolates,
    //! e.g. `linear`) or it rounds the position onto integer grid points (`select`, e.g.
    //! `nearest`/`floor`/`ceil`/`round`). `none` only accepts integral indices.
    static constexpr bool fractional =
        interpolates || (kwk::__::fractional_capable_v<Interpolators> || ...);

    //==============================================================================================================
    // Uniform construction backbone : every option follows the same "field over itself" protocol
    // (element_type / type / identifier_type / label_type / label() / operator()(identifier)),
    // so an accessor composes naturally in a properties pack alongside the other options. The
    // identifier overload is distinct from the read pipeline's operator()(Position, Shape, Stride,
    // Read) below.
    //==============================================================================================================
    using element_type    = accessor_t;
    using type            = accessor_t;
    using identifier_type = __::accessor_id;
    using label_type      = kumi::str;

    template<typename Self> constexpr auto&& operator()(this Self&& self, identifier_type const&)
    {
      using self_t = std::conditional_t<std::is_const_v<std::remove_reference_t<Self>>,
                                        accessor_t const,
                                        accessor_t>;
      return std::forward_like<Self>(static_cast<self_t&>(self));
    }

    //==================================================================================================================
    // An accessor is a property over itself : it exposes its own value through an `accessor()`
    // accessor.
    //==================================================================================================================
    template<typename Self> constexpr decltype(auto) accessor(this Self&& self) noexcept
    {
      using type = decltype(std::forward_like<Self>(std::declval<accessor_t>()));
      return static_cast<type>(self);
    }

    static constexpr label_type label() { return kumi::str {"Accessor"}; }

    constexpr accessor_t(Coordinate c, Boundaries b, Interpolators... is)
        : Coordinate(KWK_FWD(c))
        , Boundaries(KWK_FWD(b))
        , Interpolators(KWK_FWD(is))...
    {
    }

    //! Friendship: the `|` pipeline composition operator needs access to the (private) stage bases
    //! so it can forward every stage of the two operands into the composite pipeline.
    template<typename C1, typename B1, typename... I1, typename C2, typename B2, typename... I2>
    friend constexpr auto                            operator|(accessor_t<C1, B1, I1...> const& lhs,
                                                               accessor_t<C2, B2, I2...> const& rhs);

    //==================================================================================================================
    /*
      Properties
    */
    //==================================================================================================================
    template<typename Self> constexpr decltype(auto) coordinate(this Self&& self) noexcept
    {
      using type = decltype(std::forward_like<Self>(std::declval<Coordinate>()));
      return static_cast<type>(self);
    }

    template<typename Self> constexpr decltype(auto) boundaries(this Self&& self) noexcept
    {
      using type = decltype(std::forward_like<Self>(std::declval<Boundaries>()));
      return static_cast<type>(self);
    }

    //==================================================================================================================
    /*
      The flat index computed from a position: shift the origin (base_index, if any), apply the
      boundary remapping, then run the coordinate strategy to recover the flat storage index.

      Shift-before-boundary is load-bearing : a boundary condition must wrap *physical* (shifted,
      0-based) indices, not logical ones. Wrapping a logical index first and then subtracting the
      base again would push an otherwise in-range index out of bounds (an OOB read), or
      double-correct a sample.
    */
    //==================================================================================================================
    template<typename Self,
             kumi::concepts::product_type Position,
             kwk::concepts::shape         Shape,
             kwk::concepts::stride        Stride>
    KWK_ABI constexpr decltype(auto)
    flat_index(this Self&& self, Position&& p, Shape&& sp, Stride&& sd)
    {
      using coordinate_ = std::remove_cvref_t<Coordinate>;
      auto shifted      = shift_position<coordinate_>(KWK_FWD(p));
      if constexpr(kumi::concepts::unit_type<Boundaries>)
        return self.coordinate()(KWK_FWD(shifted), KWK_FWD(sd));
      else return self.coordinate()(self.boundaries()(KWK_FWD(shifted), KWK_FWD(sp)), KWK_FWD(sd));
    }

    // Pre-stage : shift the origin per the coordinate strategy (identity for canonical/0-based).
    template<typename Coordinate_, kumi::concepts::product_type Position>
    static constexpr decltype(auto) shift_position(Position&& p)
    {
      if constexpr(requires { Coordinate_::shift(KWK_FWD(p)); })
        return Coordinate_::shift(KWK_FWD(p));
      else return KWK_FWD(p);
    }

    // Apply the boundary remapping (identity when absent).
    template<typename Self, kumi::concepts::product_type Position, kwk::concepts::shape Shape>
    static constexpr auto bounded(Self&& self, Position&& p, Shape&& sp)
    {
      if constexpr(kumi::concepts::unit_type<Boundaries>) return KWK_FWD(p);
      else return static_cast<Boundaries&>(KWK_FWD(self))(KWK_FWD(p), KWK_FWD(sp));
    }

    //==================================================================================================================
    /*
      Forward (down) stage: position/indices -> a sample request.

      The (boundary-mapped) position is walked through the interpolator pack left-to-right (see
      `kwk::__::accessor_down_walk`). With no interpolating stage the (possibly rounded) position is
      flattened through the coordinate stage into a single flat index.
    */
    //==================================================================================================================
    template<typename Self,
             kumi::concepts::product_type Position,
             kwk::concepts::shape         Shape,
             kwk::concepts::stride        Stride>
    KWK_ABI constexpr decltype(auto) down(this Self&& self, Position&& p, Shape&& sp, Stride&& sd)
    {
      using coordinate_ = std::remove_cvref_t<Coordinate>;
      auto shifted      = shift_position<coordinate_>(KWK_FWD(p));
      auto pos          = bounded(KWK_FWD(self), KWK_FWD(shifted), KWK_FWD(sp));
      return kwk::__::accessor_down_walk<Interpolators...>::run(
          KWK_FWD(pos),
          KWK_FWD(sp),
          KWK_FWD(sd),
          [ & ](auto&& final_pos) -> decltype(auto)
          { return KWK_FWD(self).coordinate()(KWK_FWD(final_pos), KWK_FWD(sd)); });
    }

    //==================================================================================================================
    /*
      Reverse (up) stage: raw storage value(s) -> final interpolated value.

      Driven by a read callback (read(flat_index) -> raw value) so the interpolator can gather
      neighbours without knowing about the storage layout. Selection strategies return the single
      raw value (the sample is the flat index). The up stage mirrors down: it locates the first
      interpolating stage (the one that produced the sample) and applies its `up`.
    */
    //==================================================================================================================
    template<typename Self,
             typename Sample,
             typename Read,
             kwk::concepts::shape  Shape,
             kwk::concepts::stride Stride>
    KWK_ABI constexpr decltype(auto)
    up(this Self&& self, Sample&& sample, Shape&& sp, Stride&& sd, Read&& read)
    {
      (void)self;
      return kwk::__::accessor_up_walk<Interpolators...>::run(
          KWK_FWD(sample), KWK_FWD(sp), KWK_FWD(sd), KWK_FWD(read));
    }

    //==================================================================================================================
    /*
      Whole-pipeline access: position -> final value, given a storage read callback.
    */
    //==================================================================================================================
    template<typename Self,
             kumi::concepts::product_type Position,
             kwk::concepts::shape         Shape,
             kwk::concepts::stride        Stride,
             typename Read>
    KWK_ABI constexpr decltype(auto)
    operator()(this Self&& self, Position&& p, Shape&& sp, Stride&& sd, Read&& read)
    {
      return self.up(
          self.down(KWK_FWD(p), KWK_FWD(sp), KWK_FWD(sd)), KWK_FWD(sp), KWK_FWD(sd), KWK_FWD(read));
    }
  };

  //! Alias for the accessor type (natural name of the property).
  template<typename Coordinate, typename Boundaries, typename... Interpolators>
  using accessor = accessor_t<Coordinate, Boundaries, Interpolators...>;

  //====================================================================================================================
  /*
    Pipeline composition : `accessor | accessor` is right-append. The composite keeps the left
    operand's coordinate / boundary addressing prefix and concatenates the two interpolator packs
    (`Interpolators...` of the left, then of the right) onto it. Because only the first
    interpolating stage is engaged (see `accessor`), a right operand with its own interpolating
    stage only takes effect when the left operates without one. Absent stages (a `kumi::unit`
    boundary or a `none` interpolator) contribute the identity transform.
  */
  //====================================================================================================================
  template<typename C1, typename B1, typename... I1, typename C2, typename B2, typename... I2>
  constexpr auto operator|(accessor_t<C1, B1, I1...> const& lhs,
                          accessor_t<C2, B2, I2...> const& rhs)
  {
    return accessor_t<C1, B1, I1..., I2...> {lhs.coordinate(),
                                             lhs.boundaries(),
                                             static_cast<I1 const&>(lhs)...,
                                             static_cast<I2 const&>(rhs)...};
  }
}
