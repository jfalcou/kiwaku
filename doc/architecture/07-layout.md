# The Fiber: layout
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_layout The Fiber: *layout*

# The Fiber: `layout`

## What it is

A **layout** is the pair \f$\mathsf{layout} = (\,\mathsf{shape},\ \mathsf{stride}\,)\f$
of \ref kwk_arch_shape_position "shape and stride": the bounded box of positions together with
the linear step that embeds it into the base blob. It is what a collection hands to its
`\ref kwk_arch_accessor "accessor"` to resolve a logical position into memory. In the source, the
collection `operator[]` decomposes access as:

\code{.cpp}
auto &&[l, r] = self.layout()[is...];          // the layout at this point
auto idx      = self.accessor()(kumi::tuple{is...}, l); // topology resolves it
auto element  = ...;                            // into an element/offset
\endcode

i.e. the layout is the *geometry*, the accessor the *topology*, applied in that order.

## What it is mathematically

Recall the **local trivialization** of a fiber bundle (`\ref kwk_arch_foundations`):
\f$\pi^{-1}(U)\ \cong\ U\times F,\f$

where \f$U\f$ is a piece of the base and \f$F\f$ the fiber. **A layout is precisely such a local
piece plus the affine structure to address it:**

- the **shape** is the coordinate box \f$U\f$ (which positions exist),
- the **stride** is the linear embedding (an affine chart) from \f$U\f$ into the base blob,
- the **kind** (from `\ref kwk_arch_kind`) is the fiber \f$F\f$.

So `(shape, stride, kind)` together realize \f$U\times F\f$ over the blob. When a slice or view is taken,
the collection recomputes a **new layout** (`reshape`/`restride`) — i.e. a new neighborhood \f$U'\f$
with its own chart — and points the source inside the same base. This is the Grothendieck intuition:
a collection is a *family of fibers varying over its base*, and each region of it is locally a
trivial product \f$U\times F\f$ described by its own layout.

For a product `kind` (SoA / fibered regime) the layout is duplicated per field via `make_offsets`
(the per-field offset map, see \ref kwk_arch_scheme "the scheme") — the "several fibers over one base"
fibered-category generalization. Per-field *geometry* (each field its own shape) is the Phase K
sheaf extension.

## What it does

- Holds `(shape, stride)` and is the thing turned into actual byte offsets by the accessor.
- Underlies `operator[]` (scalar and slicer forms): the lazy `[l, r]` pair is split into the layout
  to *keep* (`r`) and the layout to *resolve* (`l`).
- Is rederived by views/slices, so that every sub-region remains a faithfully described fiber \f$U\times F\f$
  with its own chart, still backed by the original blob.

## How to implement it

**Status: emergent.** There is no standalone `layout` class today; layout is the *combination* of
`shape` + `stride` (both implemented in `utility/layout/`) as recomputed by slicing/tiling helpers,
and consumed positionally in `collection.hpp` and `accessor.hpp`.

**Notes and open work.**

1. **The pair is explicit in use.** The `auto [l, r] = self.layout()[is...]` decomposition in
   `collection.hpp` splits the lazy pair into the layout to *resolve* (`l`) and the layout to *keep*
   (`r`). This contract is live and tested for scalar, slicer and tiler access; the two helpers
   `shape()`/`stride()` remain the canonical single source the layout derives from.
2. **One accessor contract.** `layout()` returns the `(shape, stride)` pair consistently after every
   slice/tile; there is no longer any alternating second flavour to reconcile.
3. **Per-field layouts for product kinds.** The SoA/sheaf regime needs one layout (or offset map) per
   field; `static_owning_bytes`/`make_offsets` in `builder.hpp` are that plumbing and are live for
   the uniform-shape SoA path. Per-field *shapes* (each field a different layout) are **Phase K**.
4. **Tiling.** `collection::operator[](tiler)` applies `t(self.layout())` — a tile transforms one
   layout into another (block-structured). This is where `\ref kwk_arch_kind` product types and
   `\ref kwk_arch_accessor` tiling meet: a block becomes a coarser lattice over a product fiber.

Continue to \ref kwk_arch_accessor "the topology: accessor".
