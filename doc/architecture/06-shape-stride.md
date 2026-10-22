# Positions: shape & stride
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_shape_position Positions: *shape* & *stride*

# Positions: `shape` & `stride`

## What it is

**`kwk::shape<Dims...>`** describes the *extents* of the space in each dimension. Each `Dims` is
either a compile-time constant (static) or `kwk::_` (dynamic):

\code{.cpp}
kwk::shape<3, 4, kwk::_> s;   // two static dims (3, 4) and one dynamic dim
auto s2 = kwk::shape(2_c, 3, 4_c); // hybrid from sizes + _c literals
\endcode

It stores dimensions as a compressed hybrid tuple, exposes `size() = ∏ dims`, `ndim`, `flat_ndim`
(after flattening product sub-dimensions), `get<I>`, and equivalence/squeeze helpers.

**`kwk::stride<Dims...>`** describes the *step*, in scalar element units, taken along each dimension.
Its key invariant is the **cosize** — the total memory interval spanned:
\f$\operatorname{cosize}(\mathtt{shape}, \mathtt{stride}) = 1 + \sum_{i=0}^{N-1}(\mathtt{shape}_i - 1)\cdot\mathtt{stride}_i,\f$

implemented in `stride::cosize(shape)` via an inner product. Given a default `storage_order`
(row-major / column-major / arbitrary `generator` permutation), `to_stride(shape, order)` derives a
stride by a (exclusive) scan of the shape — the standard contiguity construction.

## What it is mathematically

`shape` and `stride` jointly name the **positions** \f$P\f$ of the container, i.e. the index set
over which the \f$X\f$-valued slots are laid out. In container terminology (`\ref
kwk_arch_foundations`):

- the **shape** \f$s : S\f$ is the set of positions itself — for an \f$N\f$-dimensional space,
  \f$P(s)=\{0,\dots,s_0-1\}\times\cdots\times\{0,\dots,s_{N-1}-1\}\f$;
- the **stride** realizes the *mapping from this grid into the linear base space* — a lattice
  embedding \f$(i_0,\dots,i_{N-1})\ \longmapsto\ \textstyle \sum_k i_k\,\mathsf{stride}_k.\f$

This is a **Z-module / monomial lattice** picture: shape gives the bounded box, stride gives the
linear form that embeds it into the base \f$\mathbb{Z}\f$. Row-major vs column-major are the two
standard bases; an arbitrary `storage_order` generator gives any linear basis, enabling
transposed / permuted / (future) space-filling layouts. Because the embedding is linear and fixed,
`squeeze`, `reshape`, `restride`, `add_rank` and `equivalent` are all earnest — they are changes of
basis / changes of the box inside the same ambient lattice.

Note that **stride does not decide access semantics**; it only says *where memory is*. Which position
is legal and how it is reached is the job of the `\ref kwk_arch_accessor "accessor"` (topology),
which consumes `(position, layout)` and applies coordinate, boundary and interpolation logic.

## What it does

- `shape`: hybrid static/dynamic extents, `size()`, `ndim`, `flat_ndim`, comparison, `squeeze`
  (drop trivial dims), `reshape` (apply slicers), `add_rank` (append a dim), `equivalent`
  (same data space after squeeze).
- `stride`: the per-dim step, `cosize(shape)` for the total interval, `to_stride(shape, order)` for
  canonical strides, `restride` with slicers, `get<I>`, comparison.
- Together they form the **`layout`** of a collection (`\ref kwk_arch_layout`): the pair
  `(shape, stride)` that the accessor and the collection `operator[]` walk.

## How to implement it

**Status: implemented and largely complete.** `include/kiwaku/utility/layout/shape.hpp` and
`stride.hpp` provide the classes, deduction guides, and the family of layout helpers above, plus the
kumi/std structured-binding protocols.

**Notes and open work.**

1. **`shape.size()`** currently `kumi::prod` of the flattened extents seeded at `fixed<1>` — correct
   for rectangular shapes; product (fielded) sub-dimensions rely on flattening. Verify `size()`
   against `flat_ndim` semantics for tiled kinds.
2. **`squeeze` / `equivalent`** drop unit dimensions; confirm they remain correct when a shape
   component is itself a product (tile) — the tiling case (`\ref kwk_arch_accessor`) treats a
   product as a block, not free dimensions.
3. **`to_stride` for arbitrary `storage_order`** uses a compile-time `generator` permutation. This is
   the hook by which a future space-filling (Morton/Hilbert) coordinate strategy (`coordinate.hpp`
   leaves `morton_t`/`hilbert_t` as empty stubs) would produce non-canonical strides.
4. **Static vs dynamic mixing** must keep `cosize` a constant expression when fully static (so a
   `stack_blob<N, align>` can be chosen by `parse_allocator`) — this coupling with
   `\ref kwk_arch_options "the builder"` is the real contract to preserve.

Continue to \ref kwk_arch_layout "the fiber: layout".
