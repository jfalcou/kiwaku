//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#pragma once

#include <kiwaku/detail/detail.hpp>

#include <cstddef>
#include <new>

namespace kwk
{
  namespace __
  {
    //==================================================================================================================
    // element: the atom of the representation.
    //
    // An element is a single point of the space: which `kind` sits at that point, its byte offset
    // in the underlying blob, and which layout/fiber it belongs to (its `Layout_idx`). Every other
    // abstraction (scheme, layout, accessor) ultimately resolves to elements.
    //==================================================================================================================
    template<typename KindT, std::size_t N = 1> struct element
    {
      //! The kind (type) of the scalar/component this element names.
      using kind_type     = KindT;
      //! The value transported at this point.
      using value_type    = KindT;
      //! Which layout/fiber of the space this element belongs to.
      using Layout_idx    = kumi::index_t<N>;

      //! Byte offset of this point inside the blob.
      std::size_t offset  = 0;

      constexpr element() = default;
      explicit constexpr element(std::size_t o) noexcept
          : offset {o}
      {
      }

      friend constexpr bool operator==(element const&, element const&) = default;
    };

    //! Resolve `(base, element)` to a correctly typed pointer into the blob.
    template<typename KindT, std::size_t N>
    constexpr KindT* resolve(element<KindT, N> const& e, std::byte* base) noexcept
    {
      return std::launder(reinterpret_cast<KindT*>(base + e.offset));
    }

    //! Resolve `(base, element)` to a correctly typed pointer (const blob).
    template<typename KindT, std::size_t N>
    constexpr KindT const* resolve(element<KindT, N> const& e, std::byte const* base) noexcept
    {
      return std::launder(reinterpret_cast<KindT const*>(base + e.offset));
    }
  }
}
