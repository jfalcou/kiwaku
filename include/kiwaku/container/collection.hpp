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
#include <kiwaku/container/properties.hpp>
#include <array>
#include <tuple>
#include <utility>

namespace kwk
{
  // `collection` is forward-declared in `kiwaku/fwd.hpp` (the single place that declares it).
  //
  // `collection` is a *variadic* template over its component types. Each component is a concrete
  // property type (storage, layout, scheme, accessor, kind) and `collection` holds them through a
  // single `properties` intermediate that simply (publicly) inherits the pack. Because a collection
  // *has* its properties, the `properties` base is inherited privately; the components are still
  // reached through `operator->` (`collection->shape()`, `collection->kind()`, ...) while
  // `operator[]` is re-exposed directly. The `layout` component couples the shape and stride; the
  // option-ordered pack is exactly the argument order of the collection constructor, so CTAD
  // deduces it directly from `kwk::collection{ storage, layout, scheme, accessor, kind }`.

  //====================================================================================================================
  /*
    collection : the concrete container-view object.

    `collection` is a *variadic* template over its component types. It publicly inherits a single
    `kwk::__::properties` object that itself publicly inherits each component type (storage, layout,
    scheme, accessor, kind), so the whole pack is reachable through the base classes. `operator->`
    returns the properties base, so `collection->shape()`, `collection->kind()`,
    `collection->get<K>()` all resolve there directly — no per-property forwarding in the shell.
    The component pack is order-independent (each option is located in `properties` by what it is),
    but the builder always assembles it in the canonical construction order.

    The pack is exactly the constructor's argument order: `kwk::collection< Storage, Layout, Scheme,
    Accessor, Kind >`. The builder (`builder_t::operator()`) and every internal sub-view (per-field
    `get<I>`, slice/tile `make_view`) construct it as `kwk::collection{ storage, layout, scheme,
    accessor, kind }`, and CTAD deduces the variadic pack from those five component arguments, the
    layout coupling the shape and stride. The component assembly is thus fixed in one place
    (`make_collection`) and identical for every collection type.

    collection is the concrete output of the builder; table (owning) and view (borrowed) factories
    both produce a collection through the builder's CTAD.
  */
  //====================================================================================================================

  namespace __
  {
    // True when every element of a product type is a (non-constant) integral — i.e. the product is
    // a *plain index tuple* rather than a slicer. Used to keep `operator[]` on index tuples from
    // capturing `kwk::slicer` arguments (whose elements are `constant`/`ratio`/`extremum`, not
    // plainly integral).
    template<kumi::concepts::product_type T,
             typename Seq = std::make_index_sequence<kumi::size_v<T>>>
    struct index_tuple_impl;

    template<kumi::concepts::product_type T, std::size_t... I>
    struct index_tuple_impl<T, std::index_sequence<I...>>
        : std::bool_constant<(std::integral<kumi::element_t<I, T>> && ...)>
    {
    };

    template<kumi::concepts::product_type T>
    inline constexpr bool index_tuple = index_tuple_impl<T>::value;
  }

  template<typename... Opts> struct collection
  {
  private:
    struct KWK_STRUCT_ABI properties_t : Opts...
    {
      // Both the layout and the (kumi-record) scheme expose a `size()` (the latter statically, as a
      // tuple/record member count) — that makes a bare `properties.size()` ambiguous. Shadow them
      // with an explicit member that reports the layout's element count, which is the instance
      // semantic callers expect from `collection->size()`.
      [[nodiscard]] KWK_ABI constexpr auto size() const noexcept { return this->layout().size(); }
      [[nodiscard]] KWK_ABI constexpr auto cosize() const noexcept
      {
        return this->layout().cosize();
      }
    } properties;

  public:
    using impl_type = properties_t;

    // The free per-field accessor `get<I>` rebuilds a scalar field view (re-pointed storage,
    // reduced scheme/kind) and so needs the shell's private `offset_storage`. The `std::get`
    // specialization forwards to it. Both are friended rather than duplicating the offsets.
    template<std::size_t I, typename... U>
    friend constexpr auto get(collection<U...> const& collec);

    // Size type of the space, inherited from the layout.
    using size_type                     = typename impl_type::size_type;

    // The scalar content type of a single element of this space.
    using value_type                    = typename impl_type::kind_t;

    // Required by the container concepts.
    using source_type                   = void;

    // The number of dimensions of this space (a compile-time constant derived from the layout).
    static constexpr size_type ndim     = impl_type::ndim;
    static constexpr size_type itemsize = impl_type::itemsize;

    //==================================================================================================================
    /*
      Construction

      `collection` is a plain value type constructed directly as `kwk::collection{ storage, layout,
      scheme, accessor, kind }`. Both the builder (`builder_t::operator()`) and every internal
      sub-view (per-field `get<I>`, slice/tile `make_view`) construct it through the single
      `make_collection` authority, so the component assembly is fixed in one place and identical for
      every collection type.

      The constructor is a forwarding template : it accepts the five component arguments in any
      value category and forwards them to the `properties` base. CTAD is driven by the explicit
      deduction guide below, which decays each deduced component so an lvalue `layout` argument does
      not inject reference types into the pack : the guide maps the five arguments onto
      `kwk::collection<Storage, Layout, Scheme, Accessor, Kind>` and is preferred over the
      constructor's implicit guide.
    */
    //==================================================================================================================
    template<typename... Args>
      requires(sizeof...(Args) == sizeof...(Opts))
    constexpr collection(Args&&... args)
        : properties(KWK_FWD(args)...)
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
    constexpr impl_type*       operator->() noexcept { return &properties; }
    constexpr impl_type const* operator->() const noexcept { return &properties; }

    //==================================================================================================================
    /*
      Access : operator[]
    */
    //==================================================================================================================
    // A tuple/record of integral indices is forwarded to the scalar overloads. Constrained to
    // *plain* index tuples so that `kwk::slicer` (a product whose components are not integral) is
    // never captured here and always reaches the slicing overload instead.
    template<kumi::concepts::product_type T>
      requires kwk::__::index_tuple<std::remove_cvref_t<T>>
    constexpr decltype(auto) operator[](T&& t) noexcept
    {
      return kumi::apply([ & ](auto&&... i) -> decltype(auto) { return (*this)[ KWK_FWD(i)... ]; },
                         KWK_FWD(t));
    }

    // Scalar access : the accessor runs the bidirectional resolve pipeline
    // (down: indices -> sample ; read storage ; up: raw -> interpolated value).
    //
    // When fewer indices than `ndim` (the flat dimension count) are provided the space is
    // *tiled*: the indices select a position in the index (grid) space and the remainder of the
    // layout is returned as a sub-collection (a tile view) over the same storage, re-pointed to
    // the selected byte offset. When enough indices are provided to fully address every leaf
    // dimension, a single scalar element is read through the accessor.
    template<typename Self, typename... Is>
      requires(sizeof...(Is) >= 1 && (std::integral<Is> && ...))
    constexpr decltype(auto) operator[](this Self&& self, Is... is) noexcept
    {
      if constexpr(sizeof...(Is) == ndim) return self.resolve((kumi::tuple {is...}));
      else if constexpr(sizeof...(Is) < ndim) return self.tile_view(is...);
      else
        static_assert(sizeof...(Is) < ndim,
                      "[KWK] - Too many indices for the number of dimensions");
    }

    // Slicing access : a range (slicer) produces a sub-collection whose layout is the reshaped /
    // re-strided view and whose storage is re-pointed to the sliced byte offset.
    template<typename Self, kwk::concepts::slicer... S>
    constexpr auto operator[](this Self&& self, S&&... s) noexcept
    {
      auto const [ position, subl ] = self->layout()[ KWK_FWD(s)... ];
      // Flatten the origin and the parent strides to compute the element offset consumed by the
      // slicer starts, scaled by the element size : the dot product of `starts` with `fs`.
      auto const fs                 = kumi::to_tuple(kumi::flatten_all(self->stride()));
      auto const starts             = kumi::to_tuple(kumi::flatten_all(position));
      auto       offset_elements    = kumi::inner_product(starts, fs, std::size_t {0});
      return self.make_view(subl.shape(), subl.stride(), offset_elements);
    }

    // Fractional access : available when the accessor can consume a non-integral position —
    // either a blending strategy (`linear`, which reads the 2^N surrounding samples) or a
    // rounding selection (`nearest`/`floor`/`ceil`/`round`, which map the position onto an
    // integer grid point). `none` accepts only integral indices (its `operator[]` is the integral
    // overloads).
    template<typename Self, typename... Is>
      requires(sizeof...(Is) >= 1 && (std::floating_point<Is> && ...) && impl_type::fractional)
    constexpr decltype(auto) operator[](this Self&& self, Is... is) noexcept
    {
      return self.resolve(kumi::tuple {is...});
    }

  private:
    // The read is wired once, uniformly : the accessor resolves the position (through any boundary
    // / interpolator) to a flat offset, which the read maps to a value. For memory storage the read
    // addresses the blob through the scheme (per-field offsets). There is no `constant_blob`
    // special-case : a function-backed space's generator is injected into the accessor as its
    // terminal stage, so the accessor *itself* terminates at the generator and this read is never
    // invoked on that path (a `constant_blob` has no `data()`, so the memory branch is guarded by a
    // `requires` that simply falls to a dead diversion for it).
    template<typename Self, typename Position>
    constexpr decltype(auto) resolve(this Self&& self, Position&& p) noexcept
    {
      return self->accessor()(KWK_FWD(p),
                              self->shape(),
                              self->stride(),
                              [ & ]<typename Index>(Index i) -> decltype(auto)
                              {
                                // A single uniform read. Memory storage addresses the blob through
                                // the scheme (per-field offsets); a function-backed space's read is
                                // *never reached* here because its generator is the accessor's
                                // terminal stage — `make_collection` statically guarantees such a
                                // storage (no `data()`) pairs with a generator-terminal accessor,
                                // so the else branch is proven unreachable rather than silently
                                // returning a stale index that happens to compile.
                                if constexpr(requires { self->blob().data(); })
                                  return self->scheme()[ self->blob().data(), i ];
                                else
                                  static_assert(kwk::__::always_false_v<Self>,
                                                "[KWK] - a storage without addressable data must "
                                                "pair with a generator-terminal accessor, so this "
                                                "memory read is unreachable.");
                              });
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
    template<typename Self, typename NewShape, typename NewStride>
    [[nodiscard]] constexpr auto make_view(this Self&       self,
                                           NewShape const&  shp,
                                           NewStride const& strd,
                                           std::size_t      offset_elements)
    {
      if constexpr(impl_type::is_product_space)
      {
        using KindT      = impl_type::kind_t;

        // The scheme is a kumi *record* (see is_record_type), so `map_index` does not apply.
        // Advance each field's stored byte offset (scheme[I]) by `offset_elements *
        // sizeof(field_I)` and rebuild the scheme from the shifted per-field offset tuple.
        auto new_offsets = [ & ]<std::size_t... I>(std::index_sequence<I...>)
        {
          return kumi::tuple {(kumi::get<I>(self->scheme()) +
                               offset_elements * sizeof(kumi::element_t<I, KindT>))...};
        }(std::make_index_sequence<kumi::size_v<KindT>> {});
        auto new_scheme = [ & ]<std::size_t... I>(std::index_sequence<I...>)
        {
          return kwk::__::scheme<KindT, kumi::element_t<I, decltype(new_offsets)>...> {
              kumi::get<I>(new_offsets)...};
        }(std::make_index_sequence<kumi::size_v<decltype(new_offsets)>> {});

        auto store = self.offset_storage(0);
        return kwk::__::make_collection(KWK_FWD(store),
                                        kwk::layout {shp, strd},
                                        KWK_FWD(new_scheme),
                                        self->accessor(),
                                        self->kind());
      }
      else
      {
        auto store = self.offset_storage(offset_elements * sizeof(value_type));
        return kwk::__::make_collection(KWK_FWD(store),
                                        kwk::layout {shp, strd},
                                        self->scheme(),
                                        self->accessor(),
                                        self->kind());
      }
    }

    template<typename Self>
    [[nodiscard]] constexpr auto offset_storage(this Self& self, std::size_t byte_offset)
    {
      if constexpr(requires { self->blob().data(); })
        return kwk::blob<kwk::shallow_blob> {const_cast<std::byte*>(self->blob().data()) +
                                             byte_offset};
      else return self.storage(); // generator (constant storage) is passed through unchanged
    }

    // Partial indexing : fewer indices than flat `ndim` select a tile within the (possibly
    // nested) space. The tile is the structure that remains after consuming `K` leaves from the
    // front of the shape/stride, and the element offset is the dot-product of the indices with
    // the leading flattened strides. The scheme/storage is re-based by that scalar offset
    // (per-field for an SoA space, see make_view).
    template<typename Self, typename... Is> constexpr auto tile_view(this Self&& self, Is... is)
    {
      constexpr auto K               = sizeof...(Is);
      auto const     fs              = kumi::to_tuple(kumi::flatten_all(self->stride()));
      auto const     idxs            = kumi::tuple {static_cast<std::size_t>(is)...};
      // The tile offset is the dot product of the indices with the leading K flattened strides
      // (inner_product needs equal-sized tuples, so `fs` is first truncated to its K-long prefix).
      auto           offset_elements = kumi::inner_product(
          kumi::extract(fs, kumi::index<0>, kumi::index<K>), idxs, std::size_t {0});

      auto const tile_shp  = kwk::__::tile_shape<K>(self->shape());
      auto const tile_strd = kwk::__::tile_stride<K>(self->stride());
      return self.make_view(tile_shp, tile_strd, offset_elements);
    }
  };

  //====================================================================================================================
  /*
    Deduction guide : CTAD for a variadic class template needs the pack mapped from the five
    component arguments. Each deduced component is decayed (`remove_cvref_t`) so an lvalue `layout`
    argument does not inject reference types into the pack; the result is
    `kwk::collection<Storage, Layout, Scheme, Accessor, Kind>`. The explicit guide is preferred over
    the constructor's implicit `collection(Opts&&...) -> collection<Opts...>` guide (which would
    keep reference types for such lvalue arguments).
  */
  //====================================================================================================================
  template<typename Storage, typename Layout, typename Scheme, typename Accessor, typename Kind>
  collection(Storage&&, Layout&&, Scheme&&, Accessor&&, Kind&&)
      -> collection<std::remove_cvref_t<Storage>,
                    std::remove_cvref_t<Layout>,
                    std::remove_cvref_t<Scheme>,
                    std::remove_cvref_t<Accessor>,
                    std::remove_cvref_t<Kind>>;

  //======================================================================================================================
  /* Per-field (SoA) access : `kwk::get<I>(c)` returns a borrowed scalar view over field I's fiber.
   */
  //======================================================================================================================
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
  template<std::size_t I, typename... Opts>
  // requires(kumi::concepts::product_type<typename impl_type::kind_t>)
  [[nodiscard]] constexpr auto get(kwk::collection<Opts...> const& collec)
  {
    using record_t = value_type<decltype(collec)>; // the elemental kind (the product type itself)
    using field_t  = kumi::element_t<I, record_t>;
    auto off       = static_cast<std::size_t>(kumi::get<I>(collec->scheme()));
    auto store     = collec.offset_storage(off);
    auto field_scheme = kwk::__::scheme<field_t> {};
    auto skind        = kwk::as<field_t>();

    // A field view keeps the parent's full shape/stride (so per-field interpolation blends the same
    // neighbourhood as the parent); only the storage is re-pointed to field I's own fiber and the
    // scheme reduced to the scalar field type.
    return kwk::__::make_collection(KWK_FWD(store),
                                    kwk::layout {collec->shape(), collec->stride()},
                                    KWK_FWD(field_scheme),
                                    collec->accessor(),
                                    KWK_FWD(skind));
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
