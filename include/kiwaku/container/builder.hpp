//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/memory/blob.hpp>
#include <kiwaku/utility/layout.hpp>
#include <kiwaku/utility/accessor.hpp>
#include <kiwaku/fwd.hpp>
#include <kiwaku/container/collection.hpp>
#include <kiwaku/settings/options.hpp>
#include <kiwaku/settings/kind.hpp>
#include <kiwaku/settings/source.hpp>
#include <kiwaku/settings/allocator.hpp>
#include <kiwaku/settings/layout.hpp>
#include <kiwaku/settings/accessor.hpp>
#include <kiwaku/settings/base_index.hpp>

namespace kwk
{
  // Helper to get the proper alignment of a buffer
  template<typename... Ts>
  using as_alignment = std::integral_constant<std::size_t, kumi::_::max(alignof(Ts)...)>;

  template<typename T> inline constexpr std::size_t get_alignment = alignof(T);

  template<kumi::concepts::product_type T>
  inline constexpr std::size_t get_alignment<T> = kumi::apply_traits_t<as_alignment, T> {};

  namespace __
  {
    //==================================================================================================================
    // Storage intent helpers
    //==================================================================================================================
    // The single construction authority for `kwk::collection` (declared in `kiwaku/fwd.hpp`).
    // Assembles a collection from its five fully-resolved components, the `layout` coupling the
    // shape and stride into one option. Both the top-level option builder and every sub-view
    // builder route their construction through this one function, so the component pack is
    // assembled in exactly one place; sub-views pass their resolved scheme / accessor / kind
    // through unchanged, preserving byte-exact view semantics.
    template<typename Storage,
             typename Layout,
             typename Scheme,
             typename Accessor,
             typename Kind>
    constexpr auto make_collection(Storage&&  store,
                                   Layout&&   layout,
                                   Scheme&&   scheme,
                                   Accessor&& acc,
                                   Kind&&     kind)
    {
      return kwk::collection {KWK_FWD(store),
                              KWK_FWD(layout),
                              KWK_FWD(scheme),
                              KWK_FWD(acc),
                              KWK_FWD(kind)};
    }

    // Round `n` up to a multiple of `a` (a power of two).
    constexpr std::size_t align_up(std::size_t n, std::size_t a) noexcept
    {
      return (n + a - 1) & ~(a - 1);
    }

    // Collapse a `maybe<Option>` to the unwrapped value type of the option it holds, or to
    // `Absent` when the bag carries nothing. The value-carrying type of a `maybe<Option>` is
    // exactly the option's `element_type` (exposed by the monad's `maybe_properties`), so this one
    // generic alias replaces the bespoke per-option trait wrappers (`boundary_of`,
    // `interpolator_of`) with idiomatic `maybe`-composition: `M::has_value` picks the branch,
    // `M::element_type` is the content.
    template<typename M, typename Absent>
    using maybe_absent_t = std::conditional_t<M::has_value, typename M::element_type, Absent>;

    // Append a stage held by a `maybe<Option>` onto a fibre chain tuple, *if* it is present. The
    // result is a fresh tuple of a possibly-larger type (constant `has_value` picks the branch at
    // compile time), so it is returned (never assigned back into a fixed-size tuple).
    template<typename Tuple, typename Maybe>
    constexpr auto append_stage(Tuple&& t, Maybe&& m)
    {
      using maybe_t = std::remove_cvref_t<Maybe>;
      if constexpr(maybe_t::has_value)
        return kumi::cat(KWK_FWD(t), kumi::tuple {KWK_FWD(m.value)});
      else return KWK_FWD(t);
    }

    // A fresh owning region of `n` bytes on the heap — the dynamic owning backend, and also the
    // defensive fallback when a bag carries neither a generator, an allocator nor a source. The two
    // heap-backed branches of the storage selection are the same construction, so they share this
    // one helper instead of duplicating the `blob<heap_allocator>` spelling.
    inline constexpr auto owning_heap = []<typename A = kwk::heap_allocator>(std::size_t n)
    { return kwk::blob<A> {A {}, n}; };

    struct extract_source_fn
    {
      template<typename T> constexpr auto operator()(T&& t) const noexcept
      {
        using qtype = decltype(source_pointer(KWK_FWD(t)));
        return kwk::as<std::remove_pointer_t<std::remove_cvref_t<qtype>>>();
      }
    } inline constexpr extract_source_type;

    // Shape of a memory-backed source option, deduced through ADL (`shape_of` on the scalar /
    // range / array / tuple source options in their home namespace).
    struct shape_of_source_fn
    {
      template<typename E> constexpr auto operator()(E&& e) const noexcept
      {
        return shape_of(KWK_FWD(e));
      }
    } inline constexpr shape_of_source;

    // Shape of a function-backed generator option. The generator's `shape_of` lives in
    // `kwk::__` (not in the option's ADL set), so it is qualified explicitly.
    struct shape_of_generator_fn
    {
      template<typename G> constexpr auto operator()(G&& g) const noexcept
      {
        return kwk::__::shape_of(KWK_FWD(g));
      }
    } inline constexpr shape_of_generator;

    // Reinterpret any per-field source pointer as a `std::byte const*` for a byte-for-byte fiber
    // copy (`kumi::map` over a fiber-bundle source).
    struct as_byteptr_fn
    {
      template<typename P> constexpr auto operator()(P p) const noexcept
      {
        return reinterpret_cast<std::byte const*>(p);
      }
    } inline constexpr as_byteptr;

    // Realise the `base_index` setting: index `Base` addresses element offset 0. Base 0 (the
    // `c_index` identity) collapses to the plain coordinate strategy; a non-zero base wraps the
    // coordinate strategy in `based_t<Base, Inner>`, the base living solely in the addressing
    // stage and never being baked onto the stored byte/field offsets.
    template<std::size_t Base, typename Coordinates>
    static constexpr decltype(auto) wrap_base(Coordinates&& coordinates)
    {
      if constexpr(Base == 0) return coordinates;
      else
      {
        using coords_t = std::remove_cvref_t<decltype(coordinates)>;
        using wrapped  = kwk::__::based_t<Base, typename coords_t::element_type>;
        (void)coordinates;
        return kwk::__::coordinate_option<wrapped> {};
      }
    }

    // Result type of a generator used as the pipeline sink. The generator "takes whatever the
    // previous stage returns" — under the canonical resolver that is a single flat index — so its
    // value type is deduced from a one-argument call (never probed with a degenerate zero-argument
    // or a per-coordinate call).
    template<typename Gen> struct generator_value_t
    {
      using type = decltype(std::declval<Gen&>()(std::size_t {}));
    };

    //==================================================================================================================
    // scheme / storage construction for the builder
    //==================================================================================================================
    // The result of `make_offsets`: the per-field (SoA) byte-offset tuple together with the total
    // byte size of the region. A named aggregate (not a `std::pair`) so the two fields are
    // self-documenting at every reader.
    template<typename OffsetTuple> struct offsets_result
    {
      OffsetTuple offsets;
      std::size_t byte_size;
    };

    // Compute the per-field (SoA) byte offsets over a region holding `count` elements and return
    // an `offsets_result` encoding (offsets_tuple, total_byte_size). Aligns each field's fiber to
    // its own alignment.
    template<typename E> static constexpr auto make_offsets(std::size_t base, std::size_t count)
    {
      if constexpr(kumi::concepts::product_type<E>)
      {
        return [ & ]<std::size_t... I>(std::index_sequence<I...>)
        {
          std::size_t offs[ sizeof...(I) ] = {};
          std::size_t cur                  = base;
          ((cur        = align_up(cur, alignof(kumi::element_t<I, E>)),
            offs[ I ]  = cur,
            cur       += count * sizeof(kumi::element_t<I, E>)),
           ...);
          return offsets_result {kumi::tuple {offs[ I ]...}, cur};
        }(std::make_index_sequence<kumi::size_v<E>> {});
      }
      else
      {
        return offsets_result {kumi::tuple {}, count * sizeof(E)};
      }
    }

    // Build the scheme<Kind, Offsets...> from the per-field offset tuple.
    template<typename E, typename OffsetTuple>
    static constexpr auto make_scheme(OffsetTuple const& offsets)
    {
      return [ & ]<std::size_t... I>(std::index_sequence<I...>)
      {
        return kwk::__::scheme<E, kumi::element_t<I, OffsetTuple>...> {kumi::get<I>(offsets)...};
      }(std::make_index_sequence<kumi::size_v<OffsetTuple>> {});
    }

    // Copy the bytes of one SoA fiber `I` of a product kind `E` from its per-field source pointer
    // (held at `kumi::get<I>(from)`) into the owning storage at `storage + scheme_offset[I]`.
    template<typename E, typename Storage, typename Scheme, typename From, std::size_t I>
    static constexpr void copy_fiber(Storage&      storage,
                                     Scheme const& scheme,
                                     From const&   from,
                                     std::size_t   count,
                                     kumi::index_t<I> const)
    {
      using field_t               = kumi::element_t<I, E>;
      std::byte const* from_bytes = kumi::get<I>(from);
      std::byte* to_bytes = storage.data() + static_cast<std::ptrdiff_t>(kumi::get<I>(scheme));
      for(std::ptrdiff_t i = 0; i < static_cast<std::ptrdiff_t>(count * sizeof(field_t)); ++i)
        to_bytes[ i ] = from_bytes[ i ];
    }

    // Assign one SoA fiber `I` of a product kind `E` from an *array-of-structures* source: `src` is
    // a pointer to the source's record elements (kind `E`), laid out interleaved (AoS). Each target
    // fiber holds `count` values of field `I`, and the I-th value of record `k` is assigned into
    // fiber `I` at index `k` (`fiber[I][k] = src[k][I]`). This is the canonical AoS -> SoA
    // transpose.
    template<typename E, std::size_t I, typename Storage, typename Scheme, typename From>
    static constexpr void
    assign_fiber_aos(Storage& storage, Scheme const& scheme, From const* src, std::size_t count)
    {
      using field_t = kumi::element_t<I, E>;
      auto* out     = reinterpret_cast<field_t*>(storage.data() +
                                                 static_cast<std::ptrdiff_t>(kumi::get<I>(scheme)));
      for(std::size_t k = 0; k < count; ++k) out[ k ] = kumi::get<I>(src[ k ]);
    }

    // Compile-time byte size of a fully-static owning region. For a uniform kind this is
    // `stride.cosize(shape) * sizeof(E)`; for a product (SoA) kind it is the sum of all per-field
    // fibers computed by `make_offsets` (each field aligned to its own alignment), which is larger
    // than `cosize * sizeof(E)` because the region lays fields end-to-end rather than interleaving
    // them.
    template<typename E, typename Shp, typename Stride>
    static constexpr std::size_t static_owning_bytes() noexcept
    {
      constexpr std::size_t count = Shp {}.size();
      if constexpr(kumi::concepts::product_type<E>) return make_offsets<E>(0u, count).byte_size;
      else return Stride {}.cosize(Shp {}) * sizeof(E);
    }

    // Copy a *fiber-bundle* source `from` (a tuple of per-field pointers) into owning SoA storage:
    // each field's fiber is copied byte-for-byte from its own pointer at `kumi::get<I>(from)`.
    template<typename E, typename Storage, typename Scheme, typename From>
    static constexpr void
    copy_fibers(Storage& storage, Scheme const& scheme, From const& from, std::size_t count)
    {
      [ & ]<std::size_t... I>(std::index_sequence<I...>)
      {
        (copy_fiber<E>(storage, scheme, from, count, kumi::index<I>), ...);
      }(std::make_index_sequence<kumi::size_v<E>> {});
    }

    // Assign an *array-of-structures* source (a pointer to records of kind `E`) into owning SoA
    // storage: the canonical AoS -> SoA transpose across every field.
    template<typename E, typename Storage, typename Scheme, typename From>
    static constexpr void assign_fibers_aos(Storage&      storage,
                                            Scheme const& scheme,
                                            From const*   src_records,
                                            std::size_t   count)
    {
      [ & ]<std::size_t... I>(std::index_sequence<I...>)
      {
        (assign_fiber_aos<E, I>(storage, scheme, src_records, count), ...);
      }(std::make_index_sequence<kumi::size_v<E>> {});
    }

    // Copy `byte_size` bytes from a memory source pointer `from` into the owning storage region —
    // the uniform-kind populate path (the region holds implicit-lifetime arithmetic bytes, so a
    // byte-for-byte copy is both initialization and population for the current case).
    template<typename Storage, typename From>
    static constexpr void
    copy_region_bytes(Storage& storage, From const* from, std::size_t byte_size)
    {
      std::byte const* from_bytes = reinterpret_cast<std::byte const*>(from);
      std::byte*       to_bytes   = storage.data();
      for(std::ptrdiff_t i = 0; i < static_cast<std::ptrdiff_t>(byte_size); ++i)
        to_bytes[ i ] = from_bytes[ i ];
    }

    // Select the concrete storage backend from what the option bag carries:
    //   - a function-backed `generator` setting -> constant_blob (no memory, on-demand);
    //   - a memory-backed `source` setting        -> shallow_blob (borrowed view);
    //   - an allocator / `deduce_allocator`      -> a fresh owning region (blob<A>);
    //   - neither                                 -> a fresh owning heap region.
    // `E` / `Shp` / `Stride` are the fully-resolved kind element, shape and stride *types* (the
    // generator branch needs `Shp::flat_ndim`, the static branch `static_owning_bytes` and the
    // `fully_static` checks), threaded in as template arguments so the function stays
    // self-contained — no captures. Ownership is requested when either `deduce_allocator` (the
    // `table` factory) or an explicit `allocator` option is present. Allocators are NOT routed
    // through the `maybe` monad (`maybe` demands an `element_type` member, which `stack_allocator`
    // / `heap_allocator` do not carry); the decision is pure type-level logic.
    template<typename E, typename Shp, typename Stride, typename Bag>
    static constexpr auto make_storage(Bag const& bag, std::size_t byte_size)
    {
      auto           generator   = get<kwk::generator>(bag);
      auto           src         = get<kwk::source>(bag);

      constexpr bool owns_region = decltype(get<kwk::__::deduce_allocator>(bag))::has_value ||
                                   decltype(get<kwk::allocator>(bag))::has_value;

      if constexpr(decltype(generator)::has_value)
      {
        // Function-backed terminal backend : no memory, the generator is the sink of the accessor
        // pipeline, taking whatever the previous stage returns (a single flat index under
        // canonical) — so its value type is deduced from a one-argument call.
        using Gen = std::remove_cvref_t<decltype(generator.value.content)>;
        using value_t = typename generator_value_t<Gen>::type;
        constexpr Gen gen {};
        return kwk::constant_blob<value_t, gen> {};
      }
      else if constexpr(owns_region)
      {
        // Owning : a fresh region. An explicit allocator wins; otherwise `deduce_allocator` (the
        // `table` factory) picks an inline stack blob for fully-static geometry or a heap blob for
        // dynamic geometry.
        if constexpr(decltype(get<kwk::allocator>(bag))::has_value)
        {
          using A = typename decltype(get<kwk::allocator>(bag))::element_type;
          return kwk::blob<A> {A {}, byte_size};
        }
        else if constexpr(kwk::concepts::fully_static<Stride> && kwk::concepts::fully_static<Shp>)
        {
          // Fully-static owning : a byte-for-byte inline buffer (sizeof == the static region size:
          // `N * sizeof(T)` for a uniform kind, the summed per-field fibers for a product kind).
          using A = kwk::stack_allocator<static_owning_bytes<E, Shp, Stride>(), get_alignment<E>>;
          return kwk::blob<A> {};
        }
        else
        {
          // Dynamic owning : heap allocation (sizeof == 3 * sizeof(void*)).
          return kwk::__::owning_heap(byte_size);
        }
      }
      else if constexpr(decltype(src)::has_value)
      {
        // Borrowed : shallow view over the source's own memory.
        auto* p         = source_pointer(src.value);
        using pointee_t = std::remove_pointer_t<decltype(p)>;
        return kwk::blob<kwk::shallow_blob> {
            reinterpret_cast<std::byte*>(const_cast<std::remove_const_t<pointee_t>*>(p))};
      }
      else
      {
        // Neither a generator, an allocator nor a source: fall back to a fresh owning heap region
        // so the collection is always well-formed.
        return kwk::__::owning_heap(byte_size);
      }
    }

    struct builder_t
    {
      template<typename Bag> static constexpr auto operator()(Bag const& bag)
      {
        //--- source / shape / order / stride ------------------------------------------------
        auto src          = get<kwk::source>(bag);
        auto generator    = get<kwk::generator>(bag);

        // A generator-backed space sizes itself through its own options, exactly like a source
        // does (`shape_of` on the function-backed option reports its single dynamic dimension).
        // Preference collapses monadically: explicit `of_shape` ?: source shape ?: generator shape
        // ?: the unknown single-element shape. Each `and_then` lifts its `shape_of` result into a
        // `maybe<shape>` and skips the absent backend entirely, so no hand-rolled if-constexpr
        // remains — the `value_or`s alone select the first present source of the shape.
        auto explicit_shp = get<kwk::of_shape>(bag);
        auto src_shape    = src.and_then(shape_of_source);
        auto gen_shape    = generator.and_then(shape_of_generator);

        auto shp =
            explicit_shp.value_or(src_shape.value_or(gen_shape.value_or(kwk::shape {kwk::_})));

        auto order  = get<kwk::storage_order>(bag).value_or(kwk::row_major_order);
        auto stride = get<kwk::of_stride>(bag).value_or(kwk::to_stride(shp, order));

        //--- kind ---------------------------------------------------------------------------
        auto kind =
            get<kwk::kind>(bag).value_or(src.and_then(extract_source_type).value_or(kwk::real64));

        //--- accessor -----------------------------------------------------------------------
        auto coordinates       = get<kwk::coordinate>(bag).value_or(kwk::canonical);
        auto boundaries        = get<kwk::boundary>(bag);
        auto interp            = get<kwk::interpolator>(bag);

        // The `base_index` setting (`c_index` = 0-based, `fortran_index` = 1-based) shifts the
        // index-space origin so that index `Base` addresses element offset 0. It is realised by
        // wrapping the coordinate strategy in `based_t<Base, Inner>`; the base lives solely in the
        // addressing stage and is never baked onto the stored byte/field offsets. `c_index` (Base
        // 0) is the identity and collapses to the plain `canonical` path, so existing byte-layouts
        // are untouched. The base is read off the bag with the same `maybe_absent_t` fold used for
        // the accessor stages (absent -> `index_t<0>`, whose static `value` is 0).
        using base_index_maybe = decltype(get<kwk::base_index>(bag));
        constexpr std::size_t base_value =
            maybe_absent_t<base_index_maybe, kumi::index_t<0>>::value;

        auto coordinates_final = wrap_base<base_value>(coordinates);

        using element_e        = typename decltype(kind)::element_type;
        // The accessor is a fibre of piped stages. Only *present* stages are composed : the
        // resolver (the coordinate) is always the innermost stage, and a present boundary and
        // interpolator are appended after it (each by simply piping its option). An absent
        // boundary / interpolator is *not* composed at all — there is no identity stage.
        auto accessor_stages =
            append_stage(append_stage(kumi::tuple {KWK_FWD(coordinates_final)}, boundaries), interp);

        // Convenient aliases over the shape/stride *types* (needed to select the concrete
        // stack-blob capacity for fully-static owning tables below).
        using shapetype        = std::remove_cvref_t<decltype(shp)>;
        using stridetype       = std::remove_cvref_t<decltype(stride)>;

        //--- geometry -----------------------------------------------------------------------
        constexpr bool is_product    = kumi::concepts::product_type<element_e>;
        auto           count         = shp.size();

        auto           offsets_total = make_offsets<element_e>(0u, count);
        auto           byte_size =
            is_product ? offsets_total.byte_size : (stride.cosize(shp) * sizeof(element_e));

        auto           scheme      = make_scheme<element_e>(offsets_total.offsets);

        //--- storage ------------------------------------------------------------------------
        // The concrete backend is selected from what the option bag carries (generator ->
        // constant_blob, source -> shallow_blob borrowing, allocator/`deduce_allocator` -> a fresh
        // owning region, otherwise a fresh owning heap).
        auto           storage     = make_storage<element_e, shapetype, stridetype>(bag, byte_size);

        //--- populate an owning region from a source -----------------------------------------
        // When the bag asks for ownership (an allocator / `deduce_allocator`) AND provides a
        // memory source, the freshly allocated region is filled from the source. A product (SoA)
        // kind stores each field in its own fiber, so the source is either:
        //   - a *fiber bundle* source (`tuple_option`, a tuple of per-field pointers) whose fields
        //     are already separated -> each fiber is copied byte-for-byte from its own pointer, or
        //   - an *array-of-structures* source (`range_option` over records) whose fields are
        //     interleaved in memory -> each field is assigned element-by-element into its fiber
        //     (the canonical AoS -> SoA transpose).
        // For a uniform kind the region is a plain byte-for-byte copy of the source memory.
        constexpr bool owns_region = decltype(get<kwk::__::deduce_allocator>(bag))::has_value ||
                                     decltype(get<kwk::allocator>(bag))::has_value;
        if constexpr(owns_region && decltype(src)::has_value && is_product)
        {
          auto sp = source_pointer(src.value);
          if constexpr(kumi::concepts::product_type<decltype(sp)>)
          {
            // Fiber-bundle source : `sp` is a tuple of per-field pointers, one over each field.
            auto from = kumi::map(as_byteptr, sp);
            copy_fibers<element_e>(storage, scheme, from, count);
          }
          else
          {
            // Array-of-structures source : `sp` points at the first record element (kind `E`).
            using record_t    = std::remove_pointer_t<decltype(sp)>;
            auto* src_records = reinterpret_cast<record_t const*>(sp);
            assign_fibers_aos<element_e>(storage, scheme, src_records, count);
          }
        }
        else if constexpr(owns_region && decltype(src)::has_value)
        {
          copy_region_bytes(storage, source_pointer(src.value), byte_size);
        }

        //--- construct the collection -----------------------------------------------------------
        // The builder always produces a `kwk::collection`, constructed directly (no funnel).
        // Its template arguments are deduced from the call (CTAD on the five component arguments):
        // the component order is fixed (storage, layout, scheme, accessor, kind) so two
        // bags with the same components in any option order yield exactly the same concrete type.
        // The shape and stride are coupled into a single `layout` option.
        //
        // The accessor is assembled from the folded stages (resolver + optional boundary +
        // optional interpolator) — the final fibre type is whatever the fold produced.
        auto accessor_option = __::accessor_from_tuple(accessor_stages);
        return kwk::__::make_collection(KWK_FWD(storage),
                                        kwk::layout {shp, stride},
                                        KWK_FWD(scheme),
                                        KWK_FWD(accessor_option),
                                        kind);
      }
    };
  }

  inline constexpr __::builder_t builder {};
}
