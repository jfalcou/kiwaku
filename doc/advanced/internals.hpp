#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_internals Under the hood

A KIWAKU collection is a small concrete object composed from **six components** — storage,
shape, stride, scheme, accessor and kind — that the builder assembles from the options you
pass. Understanding how those components fit explains the footprint guarantees, the
struct-of-arrays layout, and where the library lets you plug in your own types. (The full
design rationale lives in the architecture documents under `doc/architecture/`.)

@section kwk_internals_layout Memory layout

A collection is an n-dimensional space whose elements live in **storage** and are
addressed through **geometry** (a shape + stride) and a **scheme** (how fields map to byte
offsets). Concretely it is `kwk::__::properties`, which owns six typed components:

| Component | Role |
|-----------|------|
| `Storage` | the raw bytes: a **blob** backend |
| `Shape`   | per-axis extents (compile-time `_c` or runtime) |
| `Stride`  | byte/element distance between consecutive cells per axis |
| `Scheme`  | per-field byte offsets — the struct-of-arrays map for a product kind |
| `Accessor`| the addressing pipeline (coordinate → boundary → interpolator) |
| `Kind`    | the element type (`kwk::as<T>()`) |

The `properties` type **inherits** (rather than member-stores) these components, so the
empty ones collapse away via the empty-base optimization: only the storage and any
dynamic shape occupy per-object state. The result is footprint-guaranteed views — a
fully-static 1-D view is a single pointer, a dynamic 1-D view is a pointer plus one
size.

**Storage backends** are blobs chosen by the builder from the options:

- `blob<shallow_blob>` — a borrowed pointer (a `view`), never owns, never copies;
- `blob<stack_blob>` — inline storage for a static (`_c`) owning `table`, no heap;
- `blob<heap_allocator>` — a heap region for a runtime-shaped owning `table`;
- `blob<constant_blob>` — no memory at all, a `kwk::generator` function evaluated on
  demand.

**Struct-of-arrays.** For a product kind (`kwk::as<kumi::tuple<T...>>()`), each field is
a contiguous **fibre** at its own byte offset `scheme[I]` in the shared region, rather
than interleaved records. That is why slicing/tiling an SoA collection re-bases each
field fibre individually (the per-field scheme advances by the element's own size, not by
the record size), and why `kwk::get<I>` yields a scalar collection over one fibre that
aliases the parent.

@section kwk_internals_dispatch Dispatch and customisation points

Dispatch is resolved at compile time — there is no virtual table. The **accessor** is a
variadic strategy pipeline the collection runs on every read:

```
position  ->  coordinate  ->  boundary  ->  interpolator  ->  storage read
```

Each stage is a `constexpr` strategy type, and an absent stage is the identity (`canonical`,
no boundary, no interpolation). The coordinate option (`canonical`, `c_index` /
`fortran_index` via base index) positions the origin; the boundary (`clamp`/`tile`/
`mirror`) answers out-of-domain reads; the interpolator (`linear`/`nearest`/`floor`/
`ceil`/`round`) consumes fractional positions. The pipeline is bidirectional — the
accessor *solves* a position to a flat index/downsampling and reads storage, and it
*un-solves* a raw sample back to an interpolated value — the identity transforms for the
absent stages.

The principal **customisation points** are:

- **Custom source** — any type you wrap is just memory: provide `kwk::source = X` where X
  is a pointer / C array / contiguous range / tuple of per-field pointers. The builder
  finds the pointer and extent through ADL (`source_pointer`, `shape_of`).
- **Custom slicer** — any type modeling `kwk::concepts::slicer` (a free `to_slicer(t)`
  findable by ADL) can be passed to `operator[]`; see @subpage kwk_slicing.
- **Custom strategy stages** — a boundary or interpolator that implements the accessor
  stage contract plugs into the pipeline without touching storage or layout.
- **Custom allocator** — storage selection honours an explicit allocator option, so an
  owning `table` can be backed by a caller-provided pool rather than the default heap.

@section kwk_internals_extending Extending the library

Extensions are type composition, not registration. Because a collection is built from
six concrete component types, most "new feature" requests reduce to providing a new
component:

- **A new element kind** is just a C++ type — pass `kwk::as<MyType>()` (compound, record,
  SIMD) and give it a storage/scheme that matches (per-field fibres for a product).
- **A new addressing behaviour** (e.g. a different boundary rule or rounding mode) is a
  new strategy type implementing the relevant stage's contract, then passed as an option.
- **A new source form** is a type plus `source_pointer`/`shape_of` overloads found by
  ADL; wrapping foreign contiguous memory then needs no library change at all
  (see @subpage kwk_interoperability).

This is the deliberate opposite of a plugin system: there is no opaque engine to extend,
because the "engine" is the composition of your concrete types. The seams above (storage
blob, scheme offsets, accessor stages, source/allocator/slicer options) are exactly where
KIWAKU ends and your code takes over.

**/