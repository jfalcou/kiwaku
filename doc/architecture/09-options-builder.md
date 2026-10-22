# Composition: options & builder
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_options Composition: *options* & *builder*

# Composition: `options` & `builder`

## What it is

**`kwk::options<Flags, Values>`** is a settings bag: a `kumi::record` of *named values* plus a
compile-time **bitfield** of *activated flags*:

\code{.cpp}
template<int Flags, kumi::concepts::product_type Values> struct options : Values { ... };
\endcode

Values are retrieved by identifier via `opts[identifier]` (yielding `maybe<T>`), and flags via
`contains`/`validate<T>()`. It catches duplicate/unnamed options with a deleted specialization.

**`kwk::builder<T>`** is the **factory** that turns an `options` bag into a concrete object of
template `T` (`kwk::collection` today). It performs *defaulting, deduction and congruence checking*, e.g.:

\code{.cpp}
auto src = get<kwk::source>(bag);                     // source pointer, may be empty
auto shp = get<kwk::of_shape>(bag).value_or(shape_of(src)); // default shape from source
auto order = get<kwk::storage_order>(bag).value_or(kwk::row_major_order);
auto stride = get<kwk::of_stride>(bag).value_or(to_stride(shp, order));
auto type = get<kwk::kind>(bag).value_or(extract kind from source).value_or(kwk::real64);
auto allocator = deduce(parse_allocator(shp,stride)).value_or(get<kwk::allocator>(bag));
auto coordinates = get<kwk::coordinate>(bag).value_or(kwk::canonical);
auto boundaries  = get<kwk::boundary>(bag);
auto interpolator= get<kwk::interpolator>(bag);
// congruence: shape vs kind and shape vs stride (one shared shape today;
// per-field congruence is Phase K)
// filter-out missing, then aggregate into T{opts...}
\endcode

The `view` and `table` factories (`container/view.hpp`, `container/table.hpp`) are thin front-ends
over `builder<kwk::collection>`; `view` forbids an allocator, `table` owns (deduces an allocator,
allocates, initializes, then `copy`s from a temporary view so the caller gets a container, not a view).

## What it is mathematically

This is **descent** — the categorical machinery that assembles a *global object* from a family of
*compatible local specifications* (see \ref kwk_arch_foundations). The steps in `builder_t::operator()`
are literally a descent / sheaf-assembly sequence:

1. **Local data**: every option is a local datum \f$x\in F(U)\f$ (a "specification over some region").
2. **Completion (defaulting)**: absent data is *uniquely filled* from consistent sources
   (`shape` from source, `stride` from shape+order, `kind` from source, `coordinate`=canonical, …).
   This is the sheaf's guarantee that missing-but-forced local sections glue uniquely.
3. **Compatibility (congruence)**: the `static_assert(congruent<...>)` checks enforce that the local
   pieces agree on overlaps — exactly the sheaf *matching* condition
   \f$f_i|_{U_i\cap U_j} = f_j|_{U_i\cap U_j}\f$.
4. **Gluing**: filtering the *missing* options and aggregating the remainder into `T{...}` is the
   glueing step — the unique global object obtained from the coherent local family.

The `options<Flags, Values>` bag is the **site**: it fixes *which identifiers exist* (the open
regions/topology of the specification language) and *which flags are set* (which structure is active).
The compile-time bitfield of flags is the discrete analogue of a Grothendieck topology / sieve on the
specification.

## What it does

- `options`: stores named values, tracks activated flags in a bitfield, `get`/`opts[id]` access,
  rejects malformed (duplicate/unnamed) bags at compile time.
- `builder<collection>`: default-completes, deduces, congruence-checks, then *glues* a
  `collection` (or `view`, `table`) from the bag; it also synthesizes storage (choosing
  `stack_blob` vs `heap_blob` from static-vs-dynamic shape/stride) and byte size
  (`static_owning_bytes`).
- Establishes the invariant that two collections with the same option *content* (in any order) have
  the same type (`view.hpp`: "Options are always passed in the same order ... same type").

## How to implement it

**Status: mostly complete, with a deliberate gap.** `settings/options.hpp` and `container/builder.hpp`
are functional; the remaining piece is the **sheaf (heterogeneous) regime**.

**Notes and open work.**

1. **The sheaf entry point is declared but not yet realized.** The builder's contract is: one shape
   shared by everyone, **or as many shapes as components in the (product) kind** — i.e. *"Kind
   becomes a smarter thing: for each component, store an ID which is the shape that corresponds."*
   Today the shared-shape (fiber-bundle) SoA path is fully wired; the per-field-shape (jagged/sheaf)
   branch is **Phase K**, where `make_offsets` generalizes from per-field *offsets* (uniform shape)
   to per-field *extents + offsets*.
2. **The `blob` arity is settled.** `blob<Storage>` is a single arity, with the backend selected via
   `as_storage_t<Storage>`; the builder constructs blobs through that trait (no two-parameter product
   form remains).
3. **Per-field offsets are live.** The per-field byte-offset computation (sizes `realign`ed by
   alignment, inclusive scan) is implemented as `make_offsets` and feeds the `\ref kwk_arch_scheme
   "scheme"` for the SoA/product regime — and is the seed for the Phase K per-field shapes.
4. **`storage_order`/`coordinate` congruence.** The arbitrary `storage_order` from the bag is
   consistent with the chosen `coordinate` (a space-filling coordinate implies a matching
   stride/order); `canonical` is currently the only coordinate actually wired, with `morton`/`hilbert`
   kept as deferred zero-cost tags.
5. **Backend selection is the truncation point.** `parse_allocator` returns `stack` only when
   `shape` *and* `stride` are fully static — the exact condition under which `cosize` is a constant
   expression — so the builder's static/dynamic analysis must stay in lockstep with
   `\ref kwk_arch_shape_position` and `\ref kwk_arch_blob`.

Continue to \ref kwk_arch_collection "the containers".
