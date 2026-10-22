# The Topology: accessor
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_accessor The Topology: *accessor*

# The Topology: `accessor`

## What it is

`kwk::accessor<Coordinate, Boundaries, Interpolator>` is the component that **resolves a logical
position into a concrete memory element** (and back). It is a pure pipeline — *coordinate → boundary →
interpolate → down/up* — composed by private inheritance:

\code{.cpp}
template<typename Coordinate, typename Boundaries, typename... Interpolators>
struct accessor : private Coordinate, private Boundaries, private Interpolators...
{
  // position/indices -> a sample request (shift origin, boundary-map, drive coordinate)
  constexpr decltype(auto) down(this Self&&, Position&& p, Shape&& sp, Stride&& sd) const;
  // raw storage value(s) -> final interpolated value
  constexpr decltype(auto) up(this Self&&, Sample&& sample, Shape&& sp, Stride&& sd, Read&& read) const;
  // whole pipeline: position -> final value, given a storage read callback
  constexpr decltype(auto) operator()(this Self&&, Position&& p, Shape&& sp, Stride&& sd, Read&& read) const;
};
\endcode

The signature is abbreviated: the accessor is a variadic pipeline (its `Interpolators` are a
zero-or-more pack), here shown with a single interpolator slot.

It carries **no storage**: it receives a source/layout per call and returns either a resolved
`element` (down) or turns raw memory back into values (up).

The settings surface (in `settings/accessor.hpp`) offers the `coordinate`, `boundary` and
`interpolator` identifiers, and the strategies live in `utility/accessor/`:

| stage | setting | strategies (`utility/accessor/`) |
|---|---|---|
| coordinate | `kwk::coordinate = coordinator<F>()` | `canonical` (inner product \f$p\cdot\mathsf{stride}\f$); `morton`, `hilbert` (deferred tags) |
| boundary | `kwk::boundary = bounds<F>()` | `clamp`, `tile`, `mirror` |
| interpolate | `kwk::interpolator = interpolate<F>()` | `none`, `nearest`, `linear`, `floor`, `ceil`, `round` |
| (tiling) | collection `tiler` | transforms `layout` for block access |

Its core resolution is `canonical`: \f$\operatorname{canonical}(p,\ \mathsf{stride}) = \sum_k p_k\,\mathsf{stride}_k,\f$

and the boundaries are pointwise maps \f$\mathbb{Z}^{N}\to [0,s_k)\f$ (clamp / tile / mirror)
applied to a position before it is embedded.

## What it is mathematically

The accessor is the **topology / the machinery of sections**: it is how the *abstract index domain*
(say out-of-range, negative, or non-integral coordinates) is made to land coherently in the base
space. In sheaf language (`\ref kwk_arch_foundations`) it plays the role of the sheaf's
**restriction and gluing** data:

- the **coordinate** strategy is the *functor on positions* — the rule that, given a global index,
  produces the local affine coordinate (canonical = the standard lattice form; a space-filling curve
  would be a different — locally non-affine — coordinate system);
- the **boundary** strategy is *how to glue a position to the sheaf when it falls outside the open
  region* — clamp (project to the nearest section), tile (identify the point modulo the period, i.e.
  descend to a quotient/cover), mirror (reflect across the boundary, a reflection group action).

Thus the accessor is the continuous/logical-to-discrete/concrete *descent*: it guarantees that
*every* requested index yields a single, well-defined element (germ) of the collection, the way a
sheaf's gluing axiom guarantees a unique global section from compatible local data.

Crucially — and this is a design invariant — **the stride never determines access legality**: the
stride only embeds memory (`\ref kwk_arch_layout`); the accessor decides *which* positions are
legal and *how* to reach them. This separation (geometry vs topology) is what lets a single layout
support many access policies and keeps the mathematics honest.

## What it does

- Composes `Coordinate` and `Boundaries` (plus, ultimately, `Storage` retrieval) by inheritance,
  giving optional fan-out: with no boundary (unit type) it forwards straight to the coordinate.
- `operator()(position, layout/shape, stride)`: applies boundary (if any), then coordinate, returning
  the resolved offset/element into the blob.
- Is a *setting*: users pick coordinate/boundary per collection in the options bag.

## How to implement it

**Status: implemented and compiling.** `utility/accessor/` provides the stages and
`utility/accessor.hpp` assembles them into the `accessor` pipeline; `settings/accessor.hpp` exposes
the identifiers. The whole pipeline is tested (coordinates, boundaries, interpolators, down/up).

**Notes and open work.**

1. **Retrieval is a two-way pipeline, not a blob owner.** The accessor resolves positions
   (`flat_index` → bounding/selection → `down`) and reconstructs values (`up`); it has **no `Storage`
   member** — it receives a source/layout per call and lets `collection::operator[]` own the blob.
   This keeps the accessor stateless and composable; `flat_index` and a `source_at`-style element
   resolution are the concrete retrieval path on top of `\ref kwk_arch_blob`.
2. **Boundaries are per-dimension maps** — they stay total (\f$\mathbb{Z}\to[0,s)\f$) and
   `constexpr`; coverage already handles product/tile positions (recursive pointwise handling for
   nested `product_type`).
3. **Interpolators are an extra pipeline stage.** The `interpolator` setting composes after the
   boundary stage: `none`/`nearest` pick a single element, `linear` blends neighbors, and the
   `floor`/`ceil`/`round` quantizers are first-class strategies — giving the non-element-valued reads
   the `view`/`table` factories expose.
4. **Space-filling coordinates.** `morton_t`/`hilbert_t` are kept as zero-cost strategy tags but not
   yet implemented; the intended integration is `to_stride`'s arbitrary `storage_order` hook
   (`\ref kwk_arch_shape_position`) plus a bit-interleaving coordinate, so they add without touching
   the layout.
5. **Tiling contract.** `collection::operator[](tiler)` calls `t(self.layout())` then rebuilds a
   layout; tiling is exercised at the collection layer and reuses the same topology rather than a
   separate accessor path.

Continue to \ref kwk_arch_options "composition: options & builder".
