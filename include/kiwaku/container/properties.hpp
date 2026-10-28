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

  }
}
