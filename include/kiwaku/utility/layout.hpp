//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/utility/layout/storage_order.hpp>
#include <kiwaku/utility/layout/shape.hpp>
#include <kiwaku/utility/layout/stride.hpp>

namespace kwk
{
  //====================================================================================================================
  //! @defgroup layout-utility Layout Utilities
  //! @ingroup utility
  //! @brief Layout Management utilities
  //!
  //! This module defines various functions, types and variables to handle container layout in a flexible way.
  //! There is no need to store a storage order at this level, the storage order simply being a compile time permutation 
  //! it can always be retrieved from the higher level to recompute a new layout.
  //====================================================================================================================

  namespace __
  {
    //==================================================================================================================
    // Product of the *flat* extents of a shape, kept compile-time (`kwk::constant`) whenever the shape is fully static.
    // Used to scale a stride by a block's element count so that the direct/tensor product stays contiguous.
    //==================================================================================================================
    template<kumi::concepts::product_type T>
    constexpr auto flat_extent(T const& t) noexcept
    {
      return kumi::prod(kumi::flatten_all(t), kwk::fixed<1>);
    }

    //==================================================================================================================
    // Recursively multiply every *leaf* stride by a factor, preserving the nesting structure of the input stride.
    //==================================================================================================================
    struct scale_stride_
    {
      template<typename V, typename F>
      static constexpr auto leaf(V const& v, F f)
      {
        if constexpr (kumi::concepts::product_type<V>) return scale_stride_::apply(v, f);
        else                                          return v * f;
      }

      template<typename T, typename F>
      static constexpr auto apply(T const& s, F f)
      {
        return kumi::apply(
          [&](auto&&... e) { return kwk::stride{ scale_stride_::leaf(e, f)... }; }, s);
      }

      template<typename T, typename F> constexpr auto operator()(T const& s, F f) const
      {
        return apply(s, f);
      }
    } inline constexpr scale_stride;

    //==================================================================================================================
    // Recursively divide every *leaf* stride by a factor, preserving the nesting structure of the input stride.
    // This is the inverse of `scale_stride`, used by the tiled divide `operator/`.
    //==================================================================================================================
    struct unscale_stride_
    {
      template<typename V, typename F>
      static constexpr auto leaf(V const& v, F f)
      {
        if constexpr (kumi::concepts::product_type<V>) return unscale_stride_::apply(v, f);
        else                                          return v / f;
      }

      template<typename T, typename F>
      static constexpr auto apply(T const& s, F f)
      {
        return kumi::apply(
          [&](auto&&... e) { return kwk::stride{ unscale_stride_::leaf(e, f)... }; }, s);
      }

      template<typename T, typename F> constexpr auto operator()(T const& s, F f) const
      {
        return apply(s, f);
      }
    } inline constexpr unscale_stride;

    //==================================================================================================================
    // Elementwise (Euclidean) remainder of `a`'s leaves by `b`'s leaves. `b` is broadcast: a single leaf of `b`
    // is applied against every leaf of `a`; otherwise the leaf counts must match.
    //==================================================================================================================
    template<kumi::concepts::product_type A, kumi::concepts::product_type B>
    constexpr auto leaf_remainder(A const& a, B const& b)
    {
      static constexpr auto nb_a = kumi::size_v<kumi::result::flatten_all_t<A>>;
      static constexpr auto nb_b = kumi::size_v<kumi::result::flatten_all_t<B>>;

      auto as = kumi::flatten_all(a);
      auto bs = kumi::flatten_all(b);

      if constexpr (nb_b == 1)
      {
        auto b0 = get<0>(bs);
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
          return kumi::tuple{ (get<I>(as) % b0)... };
        }(std::make_index_sequence<nb_a>{});
      }
      else
      {
        static_assert(nb_a == nb_b,
          "[KWK] - elementwise remainders require equal flat arity (or a single broadcast divisor)");
        return [&]<std::size_t... I>(std::index_sequence<I...>) {
          return kumi::tuple{ (get<I>(as) % get<I>(bs))... };
        }(std::make_index_sequence<nb_a>{});
      }
    }
  }

  //====================================================================================================================
  template<typename Shape, typename Stride> struct layout : private Shape, Stride
  {
    using size_type = kwk::config::default_size_type;

    constexpr layout(Shape const& s, Stride const& st) : Shape(s), Stride(st) {}

    //==================================================================================================================
    /*
      Properties
    */
    //==================================================================================================================
    template<typename Self> [[nodiscard]] KWK_ABI constexpr decltype(auto) shape(this Self&& self) noexcept
    {
      using type = decltype(std::forward_like<Self>(std::declval<Shape>()));
      return static_cast<type>(self);
    }

    template<typename Self> [[nodiscard]] KWK_ABI constexpr decltype(auto) stride(this Self&& self) noexcept
    {
      using type = decltype(std::forward_like<Self>(std::declval<Stride>()));
      return static_cast<type>(self);
    }

    static constexpr size_type ndim = Shape::flat_ndim;

    //static constexpr size_type flat_ndim = Shape::flat_ndim;

    [[nodiscard]] KWK_ABI constexpr size_type size() const noexcept { return shape().size(); }

    // Byte size
    [[nodiscard]] KWK_ABI constexpr size_type cosize() const noexcept { return stride().cosize(shape()); }

    //==================================================================================================================
    /*
      Extract the underlying projections associated to a dimension 
    */
    //==================================================================================================================
    template<typename Self, kwk::concepts::integral_arithmetic_value P, std::size_t... I>
    constexpr auto shape_at(this Self&& self, P p, std::index_sequence<I...>) noexcept
    {
      auto s = kumi::flatten(self.shape());
      return ((p == I ? get<I>(s) : 0) + ...);
    }

    template<typename Self, kwk::concepts::integral_arithmetic_value P> 
    constexpr auto indexes(this Self&& self, P p) noexcept
    {
      constexpr auto idxs = std::make_index_sequence<ndim>{};
      return kwk::slicer{fixed<0>, self.shape_at(p, idxs), fixed<1>};
    }

    // Shall have an operator taking a tiler

    // Slice a layout and returns a new layout
    template<typename Self, kwk::concepts::slicer...S>
    constexpr auto operator[](this Self&& self, S&&... s) noexcept
    {
      auto slicer = kumi::tuple{to_slicer(s)...};

      auto const shp  = kwk::reshape(self.shape(), slicer);
      auto const strd = kwk::restride(self.stride(), slicer);

      // origin returns an offset (a base index ???)
      auto position = kwk::origin(self.shape(), slicer);

      return kumi::tuple{position, kwk::layout{shp, strd}};
    }

    template<typename Self, typename T, typename TShape, typename TStride>
    constexpr auto operator[](this Self&& self, kwk::tiler<T, TShape, TStride> const& t) noexcept
    {
      // Transform this layout according to the tiling scheme described by the tiler
      return t.solver()(KWK_FWD(self), kwk::layout{TShape{}, TStride{}});
    }


    template<typename Self, kwk::concepts::integral_arithmetic_value... Idxs>
    constexpr auto operator[](this Self&& self, Idxs...) noexcept
    {
      constexpr auto N = kumi::index<sizeof...(Idxs)>;  
      constexpr auto split = kumi::concepts::product_type<kumi::element_t<0,Shape>> 
                            ? 1 : N;

      if constexpr( N == ndim ) return self;
      else if constexpr ( N < ndim )
      {
        auto [shl, shr] = kumi::split(self.shape()  , kumi::index<split>);
        auto [stl, str] = kumi::split(self.stride() , kumi::index<split>);
        return kumi::tuple{kwk::layout{ shl, stl }, kwk::layout{ shr, str } };
      }
      else 
        static_assert( N > ndim, "[KWK] - The number of indices does not match the number of dimensions");
    }

    //template<typename Self, auto O>
    //constexpr auto operator()(this Self&& self, storage_order_option<O>)
    //{
    //  // returns shape and stride permuted by storage order def
    //}
    //==================================================================================================================
    /*
      Layout algebra

      `layout` supports a small, mutually-inverse algebra over its (shape, stride) pair. Every operation is
      *nesting-preserving* : a nested (block/tile) sub-shape is treated as a genuine block and is never flattened
      before being combined. All operations generalize to arbitrary ranks and keep compile-time information
      (`kwk::constant` extents/strides) whenever the inputs are static.

      The four arithmetic operators form two mutually-inverse pairs :

      - `*` (tensor / direct product)  and `/` (tiled divide) : `(A * B) / B == A`.
      - `+` (block-diagonal sum)       and `-` (inverse)      : `(A + B) - B == A`.

      The two logical operators build combined layouts without arithmetic scaling :

      - `&&` builds a nested (two-block) layout, `merge(A, B) == A && B`.
      - `||` concatenates the modes of both factors into a single index space.
    */
    //==================================================================================================================

    //==================================================================================================================
    // Block-diagonal direct sum : `+`
    //==================================================================================================================
    //! @brief Block-diagonal direct sum of two layouts.
    //!
    //! The result is a *nested* layout with two top-level blocks: the first holds `lhs` (with its own stride,
    //! offset 0) and the second holds `rhs` (with its own stride, laid out right after `lhs`). Nesting is preserved
    //! verbatim: each factor remains a genuine block. `merge` and `&&` produce the same structure.
    //!
    //! Example: `A` = 2x3 (stride `(3,1)`), `B` = 4x5 (stride `(5,1)`).
    //! `A + B` shape is `{ {2,3}, {4,5} }`, stride `{ {3,1}, {5,1} }`.
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator+(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      return kwk::layout{kwk::shape{lhs.shape(), rhs.shape()},
                         kwk::stride{lhs.stride(), rhs.stride()}};
    }

    //==================================================================================================================
    // Inverse of the block-diagonal sum : `-`
    //==================================================================================================================
    //! @brief Inverse of the block-diagonal direct sum.
    //!
    //! Given `C == A + B` (a two-block nested layout), `C - B` strips the trailing (second) block `B` and returns the
    //! leading block `A`. This is the algebraic inverse of `+` on the left operand: `(A + B) - B == A`.
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator-(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      (void)rhs; // `rhs` only selects the overload; the trailing block is always stripped.
      auto [shl, shr] = kumi::split(lhs.shape(), kumi::index<1>);
      auto [stl, str_] = kumi::split(lhs.stride(), kumi::index<1>);
      return kwk::layout{as_shape(shl), as_stride(stl)};
    }

    //==================================================================================================================
    // Tensor / direct product : `*`
    //==================================================================================================================
    //! @brief Tensor (direct) product of two layouts.
    //!
    //! The result is the layout of the cartesian (tensor) product of the two index spaces, keeping both factors as
    //! nested blocks: `shape(A * B) = { shape(A), shape(B) }` and `stride(A * B) = { stride(A) * size(B), stride(B) }`.
    //! `B` is the *minor* (inner, higher-frequency) block — matching the matrix-Kronecker convention where each
    //! element of `A` expands to a scaled copy of `B` — while `A`'s modes are scaled so one step of `A` sweeps an
    //! entire `B` block. Unlike the previous pairwise map (which required equal ranks and produced a malformed,
    //! non-contiguous result), this generalizes to arbitrary ranks and yields a contiguous layout when both factors
    //! are contiguous.
    //!
    //! Example: `A` = 2x3 (stride `(3,1)`), `B` = 4x5 (stride `(5,1)`), `size(B) == 20`.
    //! `A * B` shape is `{ {2,3}, {4,5} }`, stride `{ {60,20}, {5,1} }`; `cosize == 120 == size`.
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator*(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      auto const block = __::flat_extent(rhs.shape());
      auto shp = kwk::shape{lhs.shape(), rhs.shape()};
      auto strd = kwk::stride{__::scale_stride(lhs.stride(), block), rhs.stride()};
      return kwk::layout{shp, strd};
    }

    //==================================================================================================================
    // Tiled divide : `/`
    //==================================================================================================================
    //! @brief Tiled divide, algebraic inverse of the tensor product `*`.
    //!
    //! Given `C == A * B` (a two-block tensor product) and the minor (inner) block `B`, `C / B` recovers the coarse
    //! block `A`: it strips the trailing `B` block and divides `A`'s stride back down by `size(B)`.
    //! `(C / B) * B == C` and, when `C == A * B`, `C / B == A`.
    //! Example: `(A * B) / B` recovers `A` (2x3, stride `(3,1)`).
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator/(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      auto [cshl, cshr] = kumi::split(lhs.shape(), kumi::index<1>);
      auto [cstl, cstr] = kumi::split(lhs.stride(), kumi::index<1>);

      auto const block = __::flat_extent(rhs.shape());
      auto ashp = as_shape(cshl);
      auto astr = __::unscale_stride(as_stride(cstl), block);
      return kwk::layout{ashp, astr};
    }

    //==================================================================================================================
    // Remainder / complement : `%`
    //==================================================================================================================
    //! @brief Shape remainder / complement (leftover after tiling).
    //!
    //! Returns the layout of what remains of `lhs`'s index space once it has been tiled by `rhs`: the result shape is
    //! the elementwise (Euclidean) remainder of `lhs`'s leaves modulo `rhs`'s leaves (`rhs` is broadcast when it has a
    //! single mode), and the result stride keeps `lhs`'s own strides so the leftover elements stay at their memory
    //! positions. The result is flat (the leftover is a residual, not a tile).
    //!
    //! Example: a 1D layout of 10 tiled by blocks of 3 leaves `10 % 3 == 1` element.
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator%(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      auto shp = __::leaf_remainder(lhs.shape(), rhs.shape());
      auto strd = kumi::flatten_all(lhs.stride());
      return kwk::layout{as_shape(shp), as_stride(strd)};
    }

    //==================================================================================================================
    // Logical product : merge / `&&`
    //==================================================================================================================
    //! @brief Blocked (logical) composition of two layouts.
    //!
    //! `merge(A, B)`, equivalently `A && B`, builds the nested two-block layout `{ shape(A), shape(B) }` with stride
    //! `{ stride(A), stride(B) }` — the same structure as the block-diagonal sum. It is the "make both a shape and a
    //! stride out of the two factors" build: each factor keeps its own (unscaled) extent and stride.
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto merge(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      return kwk::layout{kwk::shape{lhs.shape(), rhs.shape()},
                         kwk::stride{lhs.stride(), rhs.stride()}};
    }

    //! @brief Blocked (logical) composition, alias of `merge(A, B) == A && B` (see `merge`).
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator&&(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      return merge(lhs, rhs);
    }

    //==================================================================================================================
    // Concatenation : `||`
    //==================================================================================================================
    //! @brief Concatenation (disjoint union of modes).
    //!
    //! `A || B` concatenates the modes of both factors into a single (flat) shape and stride, preserving each factor's
    //! internal nesting. Unlike the tensor product `*`, no stride is scaled: the modes of `A` and `B` are simply
    //! juxtaposed.
    //! Example: `A` = 2x3 (stride `(3,1)`), `B` = 4x5 (stride `(5,1)`): `A || B` shape is `(2,3,4,5)`, stride
    //! `(3,1,5,1)`.
    //==================================================================================================================
    template<typename OtherShape, typename OtherStride>
    friend constexpr auto operator||(layout const& lhs, layout<OtherShape, OtherStride> const& rhs) noexcept
    {
      return kwk::layout{as_shape(kumi::cat(lhs.shape(), rhs.shape())),
                         as_stride(kumi::cat(lhs.stride(), rhs.stride()))};
    }

    //==================================================================================================================
    // Flatten / coalesce and complement helpers
    //==================================================================================================================
    /// @brief Coalesce (flatten) : merge all modes into a single flat shape/stride pair, dropping nesting.
    constexpr auto coalesce() const noexcept
    {
      return kwk::layout{as_shape(kumi::flatten_all(shape())), as_stride(kumi::flatten_all(stride()))};
    }

    /// @brief Unary `*` is a synonym for `coalesce` (flatten the nested structure).
    constexpr auto operator*() const noexcept { return coalesce(); }

    template<typename CharT, typename Traits>
    friend std::basic_ostream<CharT, Traits>& operator<<(std::basic_ostream<CharT, Traits>& os,
                                                         layout const& s) noexcept
    {
      return os << s.shape() << " : " << s.stride();
    }
  };

  template<typename Shape, typename Stride>
  layout(Shape&&, Stride&&) -> layout<std::unwrap_ref_decay_t<Shape>, std::unwrap_ref_decay_t<Stride>>;

  //====================================================================================================================
  //! @brief Builds a layout from an explicit shape and stride.
  //====================================================================================================================
  template<kwk::concepts::shape Shp, kwk::concepts::stride Str>
  constexpr auto make_layout(Shp const& s, Str const& st) noexcept
  {
    return kwk::layout{s, st};
  }

  //====================================================================================================================
  //! @brief Builds a layout from a shape and a compile-time storage order (the stride is computed canonically).
  //====================================================================================================================
  template<kwk::concepts::shape Shp, storage_order_descriptor order>
  constexpr auto make_layout(Shp const& s, storage_order_option<order> const& o) noexcept
  {
    return kwk::layout{s, kwk::to_stride(s, o)};
  }

  //====================================================================================================================
  /**
    @ingroup layout-utility
    @brief Checks whether a layout is compact (i.e. occupies exactly `size()` consecutive storage positions).

    A layout is *compact* when its stride packs the whole index space into a contiguous interval with no gaps and
    no overlap, i.e. `stride().cosize(shape()) == shape().size()`. This is the canonical CuTe meaning: a compact
    layout needs \[0, size) as its storage extent and can be stored in a plain `size()`-element buffer.

    For fully-static layouts the result is a compile-time constant and `is_compact` is usable in constant
    expressions (`static_assert(kwk::is_compact(L))`); for layouts with dynamic (runtime) extents the check is
    evaluated on the runtime values.

    ## Example
        auto row = kwk::make_layout(kwk::shape{2_c, 3_c}, kwk::row_major_order);  // stride (3,1)
        static_assert(kwk::is_compact(row));      // cosize == size == 6
        bool ok = kwk::is_compact(a_gap_layout);  // runtime check

    @tparam Shape type of the layout's shape
    @tparam Stride type of the layout's stride
    @param t the layout to analyse
    @return `true` if `t.stride().cosize(t.shape()) == t.size()`
  **/
  //====================================================================================================================
  template<typename Shape, typename Stride>
  [[nodiscard]] constexpr bool is_compact(layout<Shape, Stride> const& t) noexcept
  {
    return t.stride().cosize(t.shape()) == t.size();
  }
}

//#if !defined(KWK_DOXYGEN_INVOKED)
//template<typename Shape, typename Stride> struct std::tuple_size<kwk::layout<Shape, Stride>> : std::tuple_size<Shape>
//{
//};
//
//template<std::size_t I, typename Shape, typename Stride>
//struct std::tuple_element<I, kwk::layout<Shape, Stride>>
//  : kumi::tuple<std::tuple_element<I, Shape>, std::tuple_element<I, Stride>>
//{
//};
//#endif
