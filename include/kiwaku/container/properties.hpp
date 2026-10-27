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
#include <array>
#include <tuple>
#include <utility>

namespace kwk
{
  namespace __
  {
    //====================================================================================================================
    /*
      Option finders : reach an option inside a pack by what it *is*, not by its position.

      `properties` publicly inherits every option of its pack, so an option is reached by name: the
      layout (the shape/stride carrier), the kind, the scheme, the accessor and the storage are all
      located by a small type predicate over the pack. Each concrete option exposes exactly the
      members it is responsible for, so the finders just look for that fingerprint. This is what
      makes the public-inheritance composition natural: nothing needs to know a sibling's position,
      only what distinguishes it.
    */
    //====================================================================================================================
    // Whether `T` is the layout option (the shape/stride carrier): a `layout_t` exposes `shape()`
    // and `stride()`.
    template<typename T> struct is_layout_option : std::bool_constant<kwk::concepts::layout<T>>
    {
    };

    // Whether `T` is the kind option : a `kind_option` exposes its own `kind()` member.
    template<typename T> struct is_kind_option : std::bool_constant<requires { std::declval<T>().kind(); }>
    {
    };

    // Whether `T` is the scheme option : a `scheme` holds an `offsets_type`.
    template<typename T>
    struct is_scheme_option : std::bool_constant<requires { typename T::offsets_type; }>
    {
    };

    // Whether `T` is the accessor option : the `kwk::accessor` pipeline holds a `fractional` flag.
    template<typename T>
    struct is_accessor_option : std::bool_constant<requires { T::fractional; }>
    {
    };

    // The storage is the remaining option : a blob (borrowed/owning byte region) or a
    // `constant_blob` (function-backed, no memory). It is located by elimination : it is the only
    // option that is not the layout, the kind, the scheme nor the accessor.
    template<typename T>
    struct is_storage_option
        : std::bool_constant<!is_layout_option<T>::value && !is_kind_option<T>::value &&
                             !is_scheme_option<T>::value && !is_accessor_option<T>::value>
    {
    };

    // Pick the first type of the pack satisfying `Predicate<T>::value`.
    template<template<typename> class Predicate, typename... Opts> struct find_option;
    template<template<typename> class Predicate, typename Head, typename... Tail>
    struct find_option<Predicate, Head, Tail...>
    {
      using type =
          std::conditional_t<Predicate<Head>::value,
                             Head,
                             typename find_option<Predicate, Tail...>::type>;
    };
    template<template<typename> class Predicate> struct find_option<Predicate>
    {
      using type = void;
    };

    template<template<typename> class Predicate, typename... Opts>
    using find_option_t = typename find_option<Predicate, Opts...>::type;

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

      `properties` is a *fully variadic* aggregate that publicly inherits every option of its pack:
      `struct properties : public Opts...` — no positional base list, no `tuple_element` mapping.
      The packing order is exactly the construction order of the container, but an option is never
      *reached* by its position: each one is a public base and is located by what it is (see the
      option finders above), so the same aggregation works for any option set and in any order.

      The layout couples shape and stride into a single option (`layout_t<Shape, Stride>`); the
      scheme, accessor, kind and storage are inherited directly. Every property is therefore
      reachable through the base classes (`properties->shape()`, `properties->kind()`, ...) and the
      two optical ones (`shape()`, `stride()`, `size()`, `cosize()`, the layout algebra) float up
      from the layout base without any manual forwarding.

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
    struct KWK_STRUCT_ABI properties : public Opts...
    {
      // The five option roles, reached by what they are (see the finders above). Because
      // `properties` publicly inherits every option, these are also the concrete base classes.
      using layout_type        = find_option_t<is_layout_option, Opts...>;
      using kind_type          = find_option_t<is_kind_option, Opts...>;
      using scheme_type        = find_option_t<is_scheme_option, Opts...>;
      using accessor_type      = find_option_t<is_accessor_option, Opts...>;
      using storage_type       = find_option_t<is_storage_option, Opts...>;

      // Size type of the space, inherited from the layout option.
      using size_type          = typename layout_type::size_type;

      // The scalar content type of a single element of this space.
      using value_type         = typename kind_type::element_type;

      // The number of dimensions of this space (a compile-time constant derived from the layout).
      static constexpr size_type ndim = layout_type::ndim;

      //==================================================================================================================
      /*
        Constructors

        The constructor takes the constituents in the canonical construction order (storage,
        layout, scheme, accessor, kind) and forwards each into its inherited base. Because
        `properties` publicly inherits the pack in exactly that order, the base initializers follow
        it in the same sequence, and every option is constructed the same way — from its own
        constructor arguments.
      */
      //==================================================================================================================
      constexpr properties(storage_type       store,
                           layout_type const& lyt,
                           scheme_type        sch,
                           accessor_type      acc,
                           kind_type          k)
          : storage_type(KWK_FWD(store))
          , layout_type(lyt)
          , scheme_type(KWK_FWD(sch))
          , accessor_type(KWK_FWD(acc))
          , kind_type(KWK_FWD(k))
      {
      }

      //==================================================================================================================
      /*
        Global interface

        `properties` re-exposes nothing that its public bases already provide. Every option follows
        the same "field over itself" model (`shape_t::shape()`, `stride_t::stride()`,
        `layout_t::layout()`, `scheme_t::scheme()`, `accessor_t::accessor()`, `kind_option::kind()`,
        `blob_t::blob()`), so each one's own-name member floats up through the public inheritance and
        is reached directly (`properties->shape()`, `properties->kind()`, ...) — layout-driven
        members (`shape`, `stride`, `size`, `cosize`, the layout algebra, `indexes`) come from the
        layout base without any re-declaration here.

        The only exception is `storage()`, the *role* accessor for whichever backend sits in the pack
        : a `blob_t` exposes its own `blob()` member, but the function-backed `constant_blob` backend
        carries neither `storage()` nor `blob()`, so the storage role is reached here, uniformly.

        `size` / `cosize` are layout-driven aggregates and live on `layout_t`; the only reason they
        are disambiguated below with a re-export is that the scheme base (`compressed_tuple`, a kumi
        tuple) happens to also expose a static `kumi::tuple::size()`. Picking the layout versions here
        resolves that collision without re-coding either operation.
      */
      //==================================================================================================================
      using layout_type::size;
      using layout_type::cosize;

      [[nodiscard]] constexpr storage_type& storage() noexcept
      {
        return static_cast<storage_type&>(*this);
      }
      [[nodiscard]] constexpr storage_type const& storage() const noexcept
      {
        return static_cast<storage_type const&>(*this);
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
        requires(kumi::concepts::product_type<typename kind_type::element_type>)
      [[nodiscard]] constexpr auto get() const
      {
        using record_t    = typename kind_type::element_type;
        using field_t     = kumi::element_t<I, record_t>;
        auto off          = static_cast<std::size_t>(kumi::get<I>(this->scheme()));
        auto store        = offset_storage(off);
        auto field_scheme = kwk::__::scheme<field_t> {};
        auto skind        = kwk::as<field_t>();
        return kwk::__::make_collection(KWK_FWD(store),
                                        kwk::layout {this->layout().shape(), this->layout().stride()},
                                        KWK_FWD(field_scheme),
                                        this->accessor(),
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
        auto& acc = this->accessor();
        return acc(KWK_FWD(p),
                   this->layout().shape(),
                   this->layout().stride(),
                   [ & ](std::size_t i) -> decltype(auto) { return read_sample(*this, i); });
      }

      template<typename Position>
      constexpr decltype(auto) access_or_interpolate(Position&& p) const noexcept
      {
        auto const& acc = this->accessor();
        return acc(KWK_FWD(p),
                   this->layout().shape(),
                   this->layout().stride(),
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
          using KindT     = typename kind_type::element_type;
          auto new_scheme = [ & ]<std::size_t... I>(std::index_sequence<I...>)
          {
            return kwk::__::scheme<KindT,
                                   decltype(kumi::get<I>(this->scheme()) +
                                            0 * sizeof(kumi::element_t<I, KindT>))...> {
                (kumi::get<I>(this->scheme()) + offset_elements * sizeof(kumi::element_t<I, KindT>))...};
          }(std::make_index_sequence<scheme_type::field_count> {});
          // Borrow the parent's storage (re-point to `data()`), never pass the owning backend by
          // value: a heap backend is move-only (copy = compile error) and a stack backend would
          // silently deep-copy the whole buffer, breaking write-through aliasing. Each field's
          // byte offset lives in `new_scheme`, so all re-base relative to `data()` is carried
          // there.
          auto store = offset_storage(0);
          return kwk::__::make_collection(
              KWK_FWD(store),
              kwk::layout {shp, strd},
              KWK_FWD(new_scheme),
              this->accessor(),
              this->kind());
        }
        else
        {
          auto store = offset_storage(offset_elements * sizeof(value_type));
          return kwk::__::make_collection(KWK_FWD(store),
                                          kwk::layout {shp, strd},
                                          this->scheme(),
                                          this->accessor(),
                                          this->kind());
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
}
