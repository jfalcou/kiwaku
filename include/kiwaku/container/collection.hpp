//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/fwd.hpp>
#include <kiwaku/memory/blob.hpp>
#include <kiwaku/utility/layout.hpp>
#include <kiwaku/utility/accessor.hpp>
#include <kiwaku/settings/kind.hpp>
#include <kiwaku/settings/options.hpp>
#include <array>
#include <tuple>
#include <utility>

namespace kwk
{
  // `collection` is forward-declared in `kiwaku/fwd.hpp` (the single place that declares it).
  //
  // `collection` is a *variadic* template over its component types. Each component is a concrete
  // property type (storage, shape, stride, scheme, accessor, kind) and `collection` holds them
  // through a single `properties` intermediate that simply (publicly) inherits the pack. Because a
  // collection *has* its properties, the `properties` base is inherited privately; the components
  // are still reached through `operator->` (`collection->shape()`, `collection->kind()`, ...) while
  // `operator[]` is re-exposed directly. The option-ordered pack is exactly the argument order of
  // the collection constructor, so CTAD deduces it directly from `kwk::collection{ storage, shape,
  // stride, scheme, accessor, kind }`.
  //

  namespace __
  {
    //====================================================================================================================
    /*
      leaf / drop-tile helpers (layout-driven access)

      A tiled space is a nested product in which a "leaf" is a plain (non-product) extent. Indexing
      a tile consumes `K` leaves from the *front* of the structure; the remainder - the tile - must
      be re-expressible as a clean, well-formed shape/stride, so a remainder that collapses to a
      single nested product is unwrapped.
    */
    //====================================================================================================================
    // Number of leaves (plain, non-product atoms) in a nested product - simply the size of its
    // fully flattened form.
    template<typename T>
    inline constexpr std::size_t leaf_count =
        kumi::size_v<decltype(kumi::flatten_all(std::declval<T>()))>;

    // Drop K leaves from the front of a nested tuple, preserving the nested structure of the
    // remainder. A "leaf" is a plain (non-product) atom; the whole subtree in front of the split is
    // discarded one level at a time with kumi::cat / kumi::pop_front. A remainder that collapses to
    // a singleton nested product is left as-is and unwrapped downstream by `as_shape`/`as_stride`
    // (exactly `kumi::compress` semantics), so no bespoke unwrapping is needed here.
    template<std::size_t K, kumi::concepts::product_type T>
    constexpr auto drop_leaves(T const& t, kumi::index_t<K>)
    {
      if constexpr(K == 0 || kumi::size_v<T> == 0) return t;
      else
      {
        using elem0 = std::remove_cvref_t<kumi::element_t<0, T>>;
        if constexpr(kumi::concepts::product_type<elem0>)
        {
          constexpr std::size_t F0 = leaf_count<elem0>;
          // The split lands inside the first (nested) element : consume its K leading leaves and
          // re-attach the rest. When F0 == K this naturally yields `kumi::pop_front(t)`.
          if constexpr(F0 >= K)
            return kumi::cat(drop_leaves(kumi::get<0>(t), kumi::index_t<K> {}), kumi::pop_front(t));
          // The whole first element is consumed : drop it and continue with the remaining leaves.
          else return drop_leaves(kumi::pop_front(t), kumi::index_t<K - F0> {});
        }
        // Plain leaf : drop it (K==1 keeps the rest through the K==0 base case) and recurse.
        else return drop_leaves(kumi::pop_front(t), kumi::index_t<K - 1> {});
      }
    }

    // Drop K leaves and compute the remaining (tile) shape / stride as well-formed kwk objects.
    template<std::size_t K, kwk::concepts::shape Shape> constexpr auto tile_shape(Shape const& s)
    {
      return kwk::as_shape(drop_leaves(kumi::to_tuple(s), kumi::index<K>));
    }
    template<std::size_t K, kwk::concepts::stride Stride>
    constexpr auto tile_stride(Stride const& s)
    {
      return kwk::as_stride(drop_leaves(kumi::to_tuple(s), kumi::index<K>));
    }

    //====================================================================================================================
    /*
      properties : the internal state of a container.

      `properties` is a *variadic* type that "simply inherits the pack": it publicly inherits every
      component it owns, and the pack `Opts...` is the component list in the canonical ordering
      `Storage, Shape, Stride, Scheme, Accessor, Kind`. The layout carrier couples the bare shape
      and stride elements (position 1 and 2) into `layout<Shape, Stride>`; the scheme, accessor,
      kind and storage are inherited directly from the pack. Every property is therefore reachable
      through the base classes (`properties->shape()`, `properties->kind()`, ...).

      The components are inherited (public bases) rather than stored as members so that the empty
      ones collapse via the empty-base optimization: only the storage (a pointer or an owning
      region) and any dynamic shape part occupy per-object state. A fully-static 1D collection is
      therefore a single pointer, while a dynamic 1D collection is a pointer plus one runtime size.

      The accessor is kept free of storage - `properties` owns both and feeds the accessor a read
      callback that indexes the scheme over the blob. This keeps addressing (coordinate/boundary/
      interpolator) strictly separated from memory (blob/scheme).
    */
    //====================================================================================================================
    template<typename... Opts>
    struct KWK_STRUCT_ABI properties
        : public kwk::layout<std::tuple_element_t<1, std::tuple<Opts...>>,
                             std::tuple_element_t<2, std::tuple<Opts...>>>
        , public std::tuple_element_t<3, std::tuple<Opts...>>
        , public std::tuple_element_t<4, std::tuple<Opts...>>
        , public std::tuple_element_t<5, std::tuple<Opts...>>
        , public std::tuple_element_t<0, std::tuple<Opts...>>
    {
      using layout_type               = kwk::layout<std::tuple_element_t<1, std::tuple<Opts...>>,
                                                    std::tuple_element_t<2, std::tuple<Opts...>>>;
      using storage_type              = std::tuple_element_t<0, std::tuple<Opts...>>;
      using shape_type                = std::tuple_element_t<1, std::tuple<Opts...>>;
      using stride_type               = std::tuple_element_t<2, std::tuple<Opts...>>;
      using scheme_type               = std::tuple_element_t<3, std::tuple<Opts...>>;
      using accessor_type             = std::tuple_element_t<4, std::tuple<Opts...>>;
      using kind_type                 = std::tuple_element_t<5, std::tuple<Opts...>>;

      // Size type of the space, inherited from the layout.
      using size_type                 = typename layout_type::size_type;

      // The scalar content type of a single element of this space.
      using value_type                = typename kind_type::kind;

      // The number of dimensions of this space (a compile-time constant derived from the layout).
      static constexpr size_type ndim = layout_type::ndim;

      //==================================================================================================================
      /*
        Constructors

        The constructor takes the six constituents in the canonical pack order (storage, shape,
        stride, scheme, accessor, kind), typed positionally from the pack, and initializes each
        inherited base (the layout couples shape+stride; scheme/accessor/kind/storage are the
        remaining pack elements).
      */
      //==================================================================================================================
      constexpr properties(storage_type       store,
                           shape_type const&  shp,
                           stride_type const& str,
                           scheme_type        sch,
                           accessor_type      acc,
                           kind_type          k)
          : layout_type(shp, str)
          , scheme_type(KWK_FWD(sch))
          , accessor_type(KWK_FWD(acc))
          , kind_type(KWK_FWD(k))
          , storage_type(KWK_FWD(store))
      {
      }

      //==================================================================================================================
      /*
        Global interface
      */
      //==================================================================================================================
      [[nodiscard]] constexpr layout_type& layout() noexcept
      {
        return static_cast<layout_type&>(*this);
      }
      [[nodiscard]] constexpr layout_type const& layout() const noexcept
      {
        return static_cast<layout_type const&>(*this);
      }

      [[nodiscard]] constexpr storage_type& storage() noexcept
      {
        return static_cast<storage_type&>(*this);
      }
      [[nodiscard]] constexpr storage_type const& storage() const noexcept
      {
        return static_cast<storage_type const&>(*this);
      }

      [[nodiscard]] constexpr decltype(auto) blob() noexcept { return storage(); }
      [[nodiscard]] constexpr decltype(auto) blob() const noexcept { return storage(); }

      [[nodiscard]] constexpr accessor_type& accessor() noexcept
      {
        return static_cast<accessor_type&>(*this);
      }
      [[nodiscard]] constexpr accessor_type const& accessor() const noexcept
      {
        return static_cast<accessor_type const&>(*this);
      }

      [[nodiscard]] constexpr kind_type& kind() noexcept { return static_cast<kind_type&>(*this); }
      [[nodiscard]] constexpr kind_type const& kind() const noexcept
      {
        return static_cast<kind_type const&>(*this);
      }

      [[nodiscard]] constexpr scheme_type& scheme() noexcept
      {
        return static_cast<scheme_type&>(*this);
      }
      [[nodiscard]] constexpr scheme_type const& scheme() const noexcept
      {
        return static_cast<scheme_type const&>(*this);
      }

      [[nodiscard]] constexpr auto      shape() const noexcept { return layout().shape(); }
      [[nodiscard]] constexpr auto      stride() const noexcept { return layout().stride(); }
      [[nodiscard]] constexpr auto      size() const noexcept { return layout().size(); }
      [[nodiscard]] constexpr size_type cosize() const noexcept { return layout().cosize(); }

      // Pointer to the first byte of the underlying storage.
      [[nodiscard]] constexpr auto      data() noexcept { return storage().data(); }
      [[nodiscard]] constexpr auto      data() const noexcept { return storage().data(); }

      //==================================================================================================================
      /*
        Layout-driven access : indexes(dim) yields the range of valid indices along a dimension.
      */
      //==================================================================================================================
      template<kwk::concepts::integral_arithmetic_value P> constexpr auto indexes(P p) noexcept
      {
        return layout().indexes(p);
      }

      //==================================================================================================================
      /*
        Access : operator[]
      */
      //==================================================================================================================
      // A tuple/record of integral indices is forwarded to the scalar overloads.
      template<std::size_t N, typename... Ts>
        requires(N == sizeof...(Ts) && (std::integral<Ts> && ...))
      constexpr decltype(auto) operator[](kumi::tuple<Ts...> const& t) noexcept
      {
        return kumi::apply([ & ](auto... i) -> decltype(auto) { return (*this)[ i... ]; }, t);
      }

      // Scalar access : the accessor runs the bidirectional resolve pipeline
      // (down: indices -> sample ; read storage ; up: raw -> interpolated value).
      //
      // When fewer indices than `ndim` (the flat dimension count) are provided the space is
      // *tiled*: the indices select a position in the index (grid) space and the remainder of the
      // layout is returned as a sub-collection (a tile view) over the same storage, re-pointed to
      // the selected byte offset. When enough indices are provided to fully address every leaf
      // dimension, a single scalar element is read through the accessor.
      template<typename... Is>
        requires(sizeof...(Is) >= 1 && (std::integral<Is> && ...))
      constexpr decltype(auto) operator[](Is... is) noexcept
      {
        if constexpr(sizeof...(Is) == ndim) return access_or_interpolate(kumi::tuple {is...});
        else if constexpr(sizeof...(Is) < ndim) return tile_view(*this, is...);
        else
          static_assert(sizeof...(Is) < ndim,
                        "[KWK] - Too many indices for the number of dimensions");
      }

      template<typename... Is>
        requires(sizeof...(Is) >= 1 && (std::integral<Is> && ...))
      constexpr decltype(auto) operator[](Is... is) const noexcept
      {
        if constexpr(sizeof...(Is) == ndim)
          return static_cast<properties const&>(*this).access_or_interpolate(kumi::tuple {is...});
        else if constexpr(sizeof...(Is) < ndim) return tile_view(*this, is...);
        else
          static_assert(sizeof...(Is) < ndim,
                        "[KWK] - Too many indices for the number of dimensions");
      }

      // Slicing access : a range (slicer) produces a sub-collection whose layout is the reshaped /
      // re-strided view and whose storage is re-pointed to the sliced byte offset.
      template<kwk::concepts::slicer... S> constexpr auto operator[](S&&... s) noexcept
      {
        return slice_view(*this, kwk::to_slicer(KWK_FWD(s))...);
      }

      template<kwk::concepts::slicer... S> constexpr auto operator[](S&&... s) const noexcept
      {
        return slice_view(*this, kwk::to_slicer(KWK_FWD(s))...);
      }

      // Fractional access : available when the accessor can consume a non-integral position —
      // either a blending strategy (`linear`, which reads the 2^N surrounding samples) or a
      // rounding selection (`nearest`/`floor`/`ceil`/`round`, which map the position onto an
      // integer grid point). `none` accepts only integral indices (its `operator[]` is the integral
      // overloads).
      template<typename... Is>
        requires(sizeof...(Is) >= 1 && (std::floating_point<Is> && ...) &&
                 accessor_type::fractional)
      constexpr decltype(auto) operator[](Is... is) noexcept
      {
        return access_or_interpolate(kumi::tuple {is...});
      }

      template<typename... Is>
        requires(sizeof...(Is) >= 1 && (std::floating_point<Is> && ...) &&
                 accessor_type::fractional)
      constexpr decltype(auto) operator[](Is... is) const noexcept
      {
        return static_cast<properties const&>(*this).access_or_interpolate(kumi::tuple {is...});
      }

      //================================================================================================================
      /*
        Per-field (SoA) access : field I of a product (record) kind, exposed as a borrowed scalar
        view over field I's own fiber.

        A product kind (`kwk::as<kumi::tuple<T0,T1,...>>()`) is stored structure-of-arrays: each
        field is a contiguous fiber at its own byte offset (`scheme[I]`) in the shared storage. This
        returns a lightweight `collection` whose kind is the scalar `T_I`, whose shape/stride match
        the parent, and whose storage is a borrowed `shallow_blob` re-pointed to the field's fiber.
        The parent is not resized or copied: the two views alias the same bytes, so writing through
        field `I` of the result is visible in the parent (and vice-versa).
      */
      //================================================================================================================
      template<std::size_t I>
        requires(kumi::concepts::product_type<typename kind_type::kind>)
      [[nodiscard]] constexpr auto get() const
      {
        using record_t    = typename kind_type::kind;
        using field_t     = kumi::element_t<I, record_t>;
        auto off          = static_cast<std::size_t>(kumi::get<I>(scheme()));
        auto store        = offset_storage(off);
        auto field_scheme = kwk::__::scheme<field_t> {};
        auto skind        = kwk::as<field_t>();
        return kwk::__::make_collection(KWK_FWD(store),
                                        layout().shape(),
                                        layout().stride(),
                                        KWK_FWD(field_scheme),
                                        accessor(),
                                        KWK_FWD(skind));
      }

    private:
      // Reconstruct the coordinate tuple (flat/leaf order, row-major) that produced flat index `i`,
      // by un-flattening across the shape's dimension sizes (suffix-product arithmetic).
      template<typename Self> static constexpr auto unflatten(Self&& self, std::size_t i)
      {
        auto const flat = kumi::to_tuple(kumi::flatten_all(self.shape()));

        // Per-dimension extents : a static dimension reports its extent via `value`, a dynamic one
        // via `size()` — mapped (not hand-unpacked) to a tuple of `std::size_t` extents.
        auto       dims = kumi::map(
            [](auto const& d) -> std::size_t
            {
              using D = std::remove_cvref_t<decltype(d)>;
              if constexpr(requires { D::value; }) return static_cast<std::size_t>(D::value);
              else return static_cast<std::size_t>(d.size());
            },
            flat);

        // suffix[k] = product of the dimension sizes strictly after k (the row-major unit strides):
        // an exclusive suffix (right) scan of the extents, so suffix[k] = dims[k+1] * ... *
        // dims[N-1].
        auto suffix =
            kumi::exclusive_scan_right([](auto a, auto b) { return a * b; }, dims, std::size_t {1});

        // coordinate[k] = (i / suffix[k]) % dims[k], with the degenerate zero-extent / zero-stride
        // cases pinned to 0 instead of dividing by zero. Mapped elementwise over (dims, suffix).
        return kumi::map(
            [ & ](auto d, auto s) -> std::size_t
            {
              auto D = static_cast<std::size_t>(d);
              auto S = static_cast<std::size_t>(s);
              return (S == 0 || D == 0) ? std::size_t {0} : static_cast<std::size_t>((i / S) % D);
            },
            dims,
            suffix);
      }

      // Generator-backed (function) storage carries no memory : reconstruct the coordinates from
      // the flat index and evaluate the constant_blob source on demand.
      template<typename Self>
      static constexpr decltype(auto) generator_read(Self&& self, std::size_t i)
      {
        auto coords = unflatten(KWK_FWD(self), i);
        return kumi::apply([ & ](auto... c) -> decltype(auto) { return self.storage()(c...); },
                           coords);
      }

      // Storage read callback : address memory for a pointer blob, evaluate the generator
      // otherwise.
      template<typename Self>
      static constexpr decltype(auto) read_sample(Self&& self, std::size_t i)
      {
        if constexpr(requires { self.storage().data(); })
          return self.scheme().operator[](self.storage().data(), i);
        else return generator_read(KWK_FWD(self), i);
      }

      template<typename Position>
      constexpr decltype(auto) access_or_interpolate(Position&& p) noexcept
      {
        auto& acc = accessor();
        return acc(KWK_FWD(p),
                   layout().shape(),
                   layout().stride(),
                   [ & ](std::size_t i) -> decltype(auto) { return read_sample(*this, i); });
      }

      template<typename Position>
      constexpr decltype(auto) access_or_interpolate(Position&& p) const noexcept
      {
        auto const& acc = accessor();
        return acc(KWK_FWD(p),
                   layout().shape(),
                   layout().stride(),
                   [ & ](std::size_t i) -> decltype(auto) { return read_sample(*this, i); });
      }

      //==================================================================================================================
      /*
        Sub-view construction (layout-driven access)

        A sub-view reuses the *same* kind, accessor and storage intent as its parent; only the
        shape/stride and the byte position of the storage change. `offset_elements` is the number of
        *scalars* the sub-view is shifted from the parent's origin (the dot product of the leading
        indices/starts with the flattened strides).

        For a **uniform** space the origin shift is applied by re-pointing the (borrowed) storage to
        `data() + offset_elements * sizeof(value_type)` and keeping the (scalar-offset-free) scheme.

        For a **product** (SoA) space each field lives in its own fiber at its own byte offset
        (`scheme[I]`), and the fields have different scalar sizes — so a single storage re-point by
        `offset_elements * sizeof(record)` would address the *wrong* bytes for every field but the
        first. The origin shift is therefore applied *per field*: the storage stays at the parent's
        `data()` and each field's scheme offset advances by `offset_elements * sizeof(field_I)`. The
        shift lives purely in this sub-view's scheme, never baked onto the parent's offsets.

        Generator-backed (constant) storage carries no pointer, so for a uniform space the offset is
        dropped and the storage is passed through unchanged.
      */
      //==================================================================================================================
      template<typename NewShape, typename NewStride>
      [[nodiscard]] constexpr auto
      make_view(NewShape const& shp, NewStride const& strd, std::size_t offset_elements) const
      {
        if constexpr(scheme_type::is_product_space)
        {
          using KindT     = typename kind_type::kind;
          auto new_scheme = [ & ]<std::size_t... I>(std::index_sequence<I...>)
          {
            return kwk::__::scheme<KindT,
                                   decltype(kumi::get<I>(scheme()) +
                                            0 * sizeof(kumi::element_t<I, KindT>))...> {
                (kumi::get<I>(scheme()) + offset_elements * sizeof(kumi::element_t<I, KindT>))...};
          }(std::make_index_sequence<scheme_type::field_count> {});
          // Borrow the parent's storage (re-point to `data()`), never pass the owning backend by
          // value: a heap backend is move-only (copy = compile error) and a stack backend would
          // silently deep-copy the whole buffer, breaking write-through aliasing. Each field's
          // byte offset lives in `new_scheme`, so all re-base relative to `data()` is carried
          // there.
          auto store = offset_storage(0);
          return kwk::__::make_collection(
              KWK_FWD(store), shp, strd, KWK_FWD(new_scheme), accessor(), kind());
        }
        else
        {
          auto store = offset_storage(offset_elements * sizeof(value_type));
          return kwk::__::make_collection(KWK_FWD(store), shp, strd, scheme(), accessor(), kind());
        }
      }

      // Re-point a pointer-backed blob by `byte_offset` bytes; pass non-pointer storage through.
      // Slicing a container yields a *borrowed* shallow view over the parent's storage (which may
      // itself be an owning stack/heap buffer) at the given offset, so the offset storage is always
      // a `shallow_blob` rather than a copy of the parent's owning backend. A borrowed blob always
      // carries a mutable `std::byte*` (the builder strips source constness and enforces it through
      // the `kind`), so the `const` qualifier from the const `data()` here is shed.
      [[nodiscard]] constexpr auto offset_storage(std::size_t byte_offset) const
      {
        if constexpr(requires { storage().data(); })
          return kwk::blob<kwk::shallow_blob> {const_cast<std::byte*>(storage().data()) +
                                               byte_offset};
        else return storage();
      }

      // Slicing : the layout computes the origin (per-dimension starts) and the sliced layout.
      template<typename Self, kwk::concepts::slicer... S>
      static constexpr auto slice_view(Self&& self, S&&... sl)
      {
        auto const [ position, subl ] = self.layout()[ sl... ];
        // Flatten the origin and the parent strides to compute the element offset consumed by the
        // slicer starts, scaled by the element size : the dot product of `starts` with `fs`.
        auto const fs                 = kumi::to_tuple(kumi::flatten_all(self.stride()));
        auto const starts             = kumi::to_tuple(kumi::flatten_all(position));
        auto       offset_elements    = kumi::inner_product(starts, fs, std::size_t {0});
        return self.make_view(subl.shape(), subl.stride(), offset_elements);
      }

      // Partial indexing : fewer indices than flat `ndim` select a tile within the (possibly
      // nested) space. The tile is the structure that remains after consuming `K` leaves from the
      // front of the shape/stride, and the element offset is the dot-product of the indices with
      // the leading flattened strides. The scheme/storage is re-based by that scalar offset
      // (per-field for an SoA space, see make_view).
      template<typename Self, typename... Is> static constexpr auto tile_view(Self&& self, Is... is)
      {
        constexpr auto K               = sizeof...(Is);
        auto const     fs              = kumi::to_tuple(kumi::flatten_all(self.stride()));
        auto const     idxs            = kumi::tuple {static_cast<std::size_t>(is)...};
        // The tile offset is the dot product of the indices with the leading K flattened strides
        // (inner_product needs equal-sized tuples, so `fs` is first truncated to its K-long
        // prefix).
        auto           offset_elements = kumi::inner_product(
            kumi::extract(fs, kumi::index<0>, kumi::index<K>), idxs, std::size_t {0});

        auto const tile_shp  = kwk::__::tile_shape<K>(self.shape());
        auto const tile_strd = kwk::__::tile_stride<K>(self.stride());
        return self.make_view(tile_shp, tile_strd, offset_elements);
      }
    };
  }

  //====================================================================================================================
  /*
    collection : the concrete container-view object.

    `collection` is a *variadic* template over its component types. It publicly inherits a single
    `kwk::__::properties` object that itself publicly inherits each component type (storage, layout,
    scheme, accessor, kind), so the whole pack is reachable through the base classes. `operator->`
    returns the properties base, so `collection->shape()`, `collection->kind()`,
    `collection->get<K>()` all resolve there directly — no per-property forwarding in the shell.

    The pack is ordered exactly as the constructor: `kwk::collection< Storage, Shape, Stride,
    Scheme, Accessor, Kind >`. The builder (`builder_t::operator()`) and every internal sub-view
    (per-field `get<I>`, slice/tile `make_view`) construct it as `kwk::collection{ storage, shape,
    stride, scheme, accessor, kind }`, and CTAD deduces the variadic pack from those six component
    arguments. The component ordering is thus fixed in one place and identical for every collection
    type.

    collection is the concrete output of the builder; table (owning) and view (borrowed) factories
    both produce a collection through the builder's CTAD.
  */
  //====================================================================================================================
  template<typename... Opts> struct collection : private __::properties<Opts...>
  {
    using impl_type = __::properties<Opts...>;

    // Re-expose the object interface that `properties` provides. Under private inheritance a
    // collection *holds* its properties (it is not one), so only the members that are meant to be
    // called directly on the collection object are brought in publicly; the component interface is
    // reached through `operator->` below.
    using impl_type::operator[];

    // Size type of the space, inherited from the layout.
    using size_type                 = typename impl_type::size_type;

    // The scalar content type of a single element of this space.
    using value_type                = typename impl_type::value_type;

    // Required by the container concepts.
    using source_type               = void;

    // The number of dimensions of this space (a compile-time constant derived from the layout).
    static constexpr size_type ndim = impl_type::ndim;

    //==================================================================================================================
    /*
      Construction

      `collection` is a plain value type constructed directly as `kwk::collection{ storage, shape,
      stride, scheme, accessor, kind }`. Both the builder (`builder_t::operator()`) and every
      internal sub-view (per-field `get<I>`, slice/tile `make_view`) construct it this way, so the
      component ordering is fixed in one place and identical for every collection type.

      The constructor is a forwarding template : it accepts the six component arguments in any
      value category (an lvalue `shape` / `stride` argument included) and forwards them to the
      `properties` base. CTAD is driven by the explicit deduction guide below, which decays each
      deduced component so an lvalue `shape` / `stride` argument does not inject reference types
      into the pack : the guide maps the six arguments onto
      `kwk::collection<Storage, Shape, Stride, Scheme, Accessor, Kind>` and is preferred over the
      constructor's implicit guide.
    */
    //==================================================================================================================
    template<typename... Args>
    constexpr collection(Args&&... args)
        : impl_type(KWK_FWD(args)...)
    {
    }

    //==================================================================================================================
    /*
      Options interface : operator-> returns a pointer to the properties base so that
      collection->shape(), collection->get<K>(), collection->kind() all resolve there. The
      properties base publicly inherits every component type, so those members are reachable through
      the arrow without any forwarding declarations in this shell.
    */
    //==================================================================================================================
    constexpr impl_type*       operator->() noexcept { return static_cast<impl_type*>(this); }
    constexpr impl_type const* operator->() const noexcept
    {
      return static_cast<impl_type const*>(this);
    }

    // `operator[]` is re-exposed above via the using-declaration, so `collection[i]`,
    // `collection[...]` and `collection[sl...]` resolve directly on the object; every component
    // member (`shape`, `kind`, `get<I>`, ...) is reached through `operator->`.
  };

  //====================================================================================================================
  /*
    Deduction guide : CTAD for a variadic class template needs the pack mapped from the six
    component arguments. Each deduced component is decayed (`remove_cvref_t`) so an lvalue `shape`
    or `stride` argument does not inject reference types into the pack; the result is
    `kwk::collection<Storage, Shape, Stride, Scheme, Accessor, Kind>`. The explicit guide is
    preferred over the constructor's implicit `collection(Opts&&...) -> collection<Opts...>` guide
    (which would keep reference types for such lvalue arguments).
  */
  //====================================================================================================================
  template<typename Storage,
           typename Shape,
           typename Stride,
           typename Scheme,
           typename Accessor,
           typename Kind>
  collection(Storage&&, Shape&&, Stride&&, Scheme&&, Accessor&&, Kind&&)
      -> collection<std::remove_cvref_t<Storage>,
                    std::remove_cvref_t<Shape>,
                    std::remove_cvref_t<Stride>,
                    std::remove_cvref_t<Scheme>,
                    std::remove_cvref_t<Accessor>,
                    std::remove_cvref_t<Kind>>;

  //======================================================================================================================
  /* Per-field (SoA) access : `kwk::get<I>(c)` returns a borrowed scalar view over field I's fiber.
   */
  //======================================================================================================================
  template<std::size_t I, typename... Opts>
    requires(kumi::concepts::product_type<typename kwk::collection<Opts...>::value_type>)
  [[nodiscard]] constexpr auto get(kwk::collection<Opts...> const& c)
  {
    return c->template get<I>();
  }
}

//======================================================================================================================
/* Tuple protocol : collection reflects the kind when it is a product type. */
//======================================================================================================================
namespace std
{
  template<std::size_t I, typename... Opts>
    requires(kumi::concepts::product_type<typename kwk::collection<Opts...>::value_type> &&
             I < kumi::size_v<typename kwk::collection<Opts...>::value_type>)
  struct tuple_element<I, kwk::collection<Opts...>>
      : std::tuple_element<I, typename kwk::collection<Opts...>::value_type>
  {
  };

  template<typename... Opts>
    requires(kumi::concepts::product_type<typename kwk::collection<Opts...>::value_type>)
  struct tuple_size<kwk::collection<Opts...>>
      : std::tuple_size<typename kwk::collection<Opts...>::value_type>
  {
  };
}
