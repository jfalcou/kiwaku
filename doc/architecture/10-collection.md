# The Containers: collection, table, view
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_collection The Containers: *collection*, *table*, *view*

# The Containers: `collection`, `table`, `view`

## What it is

These are the **user-facing objects** and their factories:

- **`kwk::collection<Opts...>`** — the main container-view class: inherits its `options` (as an
  EBO-optimized `impl` aggregate) so every collection *is* a bag of `(blob, shape, stride, kind,
  accessor…)` combined with behavior.
- **`kwk::table(...)`** — factory returning an **owning** container: deduces an allocator, builds a
  view, `initialize`s and `copy`s into it.
- **`kwk::view(source, ...)`** — factory returning a **non-owning** view: forbids an allocator, wraps
  external memory via a `source` option (a pointer).
- Traits: `table_t`, `view_t` alias generation; `collection<Opts...>` with a malformed option list
  is deleted with a clear `static_assert`.

The core is `collection`'s access protocol — scalar `operator[]`, product `operator[]` (a tuple of
coords), slicer `operator[]`, tiler `operator[]` — plus `get<I>` to view a single field of a product
kind (SoA). Every access goes through `layout() → accessor() → element` and rebuilds a `builder<collection>`
for the (possibly sub-)region, so views of views stay cheap and correctly typed.

## What it is mathematically

A `collection` is the **global section** / **total object** obtained by gluing (`\ref
kwk_arch_options "the builder"`) the local `(layout, kind, accessor)` data over the base
(`\ref kwk_arch_blob "blob"`). It is the coherent, *filled-in* answer to the sheaf: for every
legal index, a definite element (germ) — \f$G(X)=\Gamma(X,F)\f$, the set of global sections of the
sheaf \f$F\f$.

- **`table`** = the **owning global section**: \f$E\f$ with its structure, materialized.
- **`view`** = a **section** of a pre-existing global object: same \f$\Gamma\f$, same structure, but
  *borrowed* base (no allocation) — an element of \f$F(U)\f$ for a region \f$U\f$ of an existing
  space.
- `get<I>` on a product kind = the projection \f$\pi_I\f$ to the \f$I\f$-th fiber/field — a new
  global section of the \f$i\f$-th component fiber bundle.
- Slicing/tiling = taking a sub-region \f$U'\subset U\f$ and *restricting* the section: the sheaf
  restriction map \f$-|\_{U'}: \Gamma(U,\cdot)\to\Gamma(U',\cdot)\f$.

So the whole stack is, functionally, **descent made concrete**: options→builder assemble the global
section; collection/view/table expose it; slices/tiles/`get<I>` are its restriction and projection
maps. This is the sense in which the library is *the* most general non-self-referential
representation of collection types: it *defines* a collection as a global section of a sheaf/fibered
structure over a base, rather than as a bespoke recursive data type.

> **A precision on "sheaf" vs "fibered."** Read literally as sheaf theory, this picture is exact for
> the **uniform / SoA** regime, where one fixed base (shape) carries one fiber (kind) — a (trivial)
> fiber bundle, glued by the builder. The proposed **jagged** regime (each field with its *own*
> shape) is not a sheaf on a single base, because each field lives over a *different* base. It is
> closer to a **fibered category / the Grothendieck construction**, or — in container language — a
> **dependent / bigraded container** (a dependent sum of per-field collections). The "sheaf" word is
> used loosely across these pages for "locally-structured, glued data"; the mathematically defensible
> core is the fibered/descent structure that applies *within* each base and the builder that assembles
> the record-level object from the per-field global sections.

## What it does

- **`view`**: `builder<collection>(options{source, opts...})` — non-owning; same canonical type for
  same option content regardless of order.
- **`table`**: deduces allocator, `builder<collection>`, `initialize`, `copy` from a filtered view —
  owning, initialized.
- **`collection`**: holds options (EBO), exposes `layout()`, `accessor()`, `blob()`, `shape()`,
  `stride()`, `data`; resolves `(coords) → element` and rebuilds sub-regions.

## How to implement it

**Status: implemented and compiling.** `collection`'s access protocol — scalar, tuple/product,
slicer, and tiler `operator[]` (each with mutable and `const` forms), plus `get<I>` for the SoA
product fields — is fully realized in `container/collection.hpp` and exercised by the test suite.

**Notes and open work.**

1. **Scalar `operator[]` is complete.** The full `layout → accessor → element → resolve<T>(blob, element)`
   chain is live (see \ref kwk_arch_element and \ref kwk_arch_accessor), with the correct
   branch between full vs partial coordinate coverage (resolving an element, or returning a builder
   for a sub-collection). Slicers and tilers rebuild a `builder<collection>` for their sub-region, so
   views of views stay cheap and correctly typed.
2. **The sheaf/bundle regime is the Phase K gate.** `collection.hpp`'s Iliffe/jagged-vs-SoA comments
   are the target for the sheaf regime; the shared-shape (fiber-bundle) SoA path is wired now, and the
   per-field-shape branch is tracked in \ref kwk_finalization_roadmap "the finalization roadmap".
3. **`get<I>` assumes product kind** — it reinterprets the `I`-th field pointer and builds a view,
   in sync with the per-field flattening (\ref kwk_arch_kind "kind") and the per-field offset map
   (`make_offsets`). Per-field *shapes* here also land with Phase K.
4. **Access-splitting helpers are live.** `indexes` (per-dimension valid-index ranges from the layout)
   and the access plumbing are implemented; `retrieve`/`source_at`-style element resolution is in the
   accessor. The tile mode where a shape entry is itself a product (a block) is already supported by
   the tiler.
5. **Deduction guides & malformed-option safety** (`collection<Ts&&...>`, deleted bad forms) are in
   place, with the `KWK_DOXYGEN_INVOKED` guards keeping diagnostics clean under Doxygen.

## Current state / roadmap

For the current status and the concrete next milestone, see the companion
**\ref kwk_finalization_roadmap "Finalization Roadmap"**. In short, the `element`→`scheme`→`blob`
stack, the per-field `make_offsets`, scalar `operator[]`, and the accessor retrieval are all done and
green; the remaining work is the per-field (sheaf/jagged) geometry of Phase K — one shape per
component, collapsing to the byte-identical SoA fast path when shapes coincide.
