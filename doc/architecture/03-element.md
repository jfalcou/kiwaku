# The Point: element
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_element The Point: *element*

# The Point: `element`

## What it is

`element` is the metadata of a **single point** of the space: which `kind` sits at that point and
*where* it lives — its byte offset — inside the `\ref kwk_arch_blob "blob"`. It is the atom of the
whole representation; everything else (`scheme`, layout, accessor) ultimately resolves to elements.

## What it is mathematically

An element is a **stalk** / **germ** of the sheaf at \f$p\f$:
\f$F_p \;=\; \varinjlim_{p\,\in\,U}\,F(U),\f$

i.e. the datum of the space "at the point \f$p\f$" (see \ref kwk_arch_foundations). In container
terms it is **one chosen slot** of a shape: the container interpretation
\f$\sum_{s:S}\prod_{p:P(s)}X\f$ picks a shape \f$s\f$, and an `element` names a particular position
\f$p\in P(s)\f$ together with its `kind` \f$X\f$. An element is thus the pair
\f$\mathrm{element}\ \approx\ (\,kind,\ offset\,)\f$ — a *region together with the datum over it*,
precisely the shape of an object \f$(U,x)\f$ in the **Grothendieck construction** \f$\int F\f$.

## What it does

- Moves from "there is a field here" (the fiber) to "this typed value lives at this byte offset."
- Carries the per-point **type** (`kind`) so the storage can be addressed and reinterpretted safely.
- Is the primitive `scheme` organizes, and the primitive the `accessor` ultimately produces.

Concretely an element answers: given the blob's base pointer, where is my value and of what type?

## How to implement it

**Status: implemented and compiling.** `include/kiwaku/memory/blob.hpp` defines

\code{.cpp}
template<typename KindT, std::size_t N = 1> struct element;
\code{.cpp}

where `KindT` is the per-point type and `N` indexes a field of a product `kind` (for a scalar point
`N == 1`). The `resolve` helpers turn `(element, blob base pointer)` into a correctly typed pointer:

\code{.cpp}
template<typename KindT, std::size_t N>
constexpr KindT* resolve(element<KindT, N> const& e, std::byte* base) noexcept;
template<typename KindT, std::size_t N>
constexpr KindT const* resolve(element<KindT, N> const& e, std::byte const* base) noexcept;
\endcode

A uniform scalar point is the element `element<Kind, 1>{ index * sizeof(Kind) }` resolved against the
blob's `data()`; a product (SoA) point resolves each field `I` as
`element<kumi::element_t<I, Kind>, I + 1>` — i.e. one element per field, each an offset into the
shared blob. Because every other abstraction reduces to elements (a `\ref kwk_arch_layout "layout"`
indexes them, an `\ref kwk_arch_accessor "accessor"` produces one, a
`\ref kwk_arch_scheme "scheme"` organizes many), `element` is the tested base of the whole stack and
compiles cleanly with the rest of `memory.hpp`.

Continue to \ref kwk_arch_scheme "the scheme".
