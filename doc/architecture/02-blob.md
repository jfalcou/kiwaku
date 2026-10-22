# The Total Space: blob
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_blob The Total Space: *blob*

# The Total Space: `blob`

## What it is

`kwk::blob<Storage>` is the raw, uninterpreted byte region that backs a collection. It is a thin
wrapper,

\code{.cpp}
template<typename Storage> struct blob : private kwk::__::as_storage_t<Storage> { ... };
\endcode

whose concrete backend is selected by the specializable variable `kwk::__::as_storage<Storage>`:

| intent | storage backend |
|---|---|
| static, stack-allocated | `stack_blob<Size, Alignment>` |
| dynamic, owning | `heap_blob` (wraps a type-erased `any_allocator`) |
| non-owning / borrowed | `shallow_blob` |

## What it is mathematically

The **base object** / **total space** \f$E\f$ of the bundle-and-sheaf picture. Like \f$B\f$ (or \f$E\f$)
in a fiber bundle (`\ref kwk_arch_foundations`), the blob carries no semantics of its own: it is
simply *"there is memory, of this extent, at this address."* All structure — which `kind` sits where,
what layout covers it, which topology reads it — is imposed by layers \f$above\f$.

The blob is the **object on which every field/fiber is based**: every `element` is an offset into it,
every `layout` indexes into it, and every `scheme` organizes `element`s *over* it. In Grothendieck
terms one may think of the blob as the underlying "space object" whose sections live in the sheaf —
the bare ground on which the whole structure is built.

## What it does

- Owns or borrows the memory; knows its **size** and **alignment**.
- Exposes the raw byte pointer for the layers above to reinterpret.
- Supports moving, `swap`, and (for owning backends) release/reallocation through the allocator.
- Is itself a `kumi::identifier` option flavor, so a `blob` can appear inside an
  `\ref kwk_arch_options "options bag"` alongside `shape`, `kind`, `source`, etc.

The selection of backend is a **trait** (`as_storage<Storage>`), so the library can map a declared
storage intent to the concrete owning/borrowing class at compile time:

\code{.cpp}
// The primary template is the borrowed fallback; each storage intent partially specializes it.
template<typename Storage>
struct as_storage { using type = kwk::shallow_blob; };                                     // borrowed

template<std::size_t Size, std::size_t Alignment>
struct as_storage<kwk::stack_allocator<Size,Alignment>> { using type = kwk::stack_blob<Size,Alignment>; }; // static owning

template<kwk::concepts::allocator A>
struct as_storage<A> { using type = kwk::heap_blob; };                                     // dynamic owning

// The concrete backend selected for a storage intent.
template<typename Storage> using as_storage_t = typename kwk::__::as_storage<Storage>::type;
\endcode

## How to implement it

**Status: complete and compiling.** `include/kiwaku/memory/blob.hpp` implements the core: the
`as_storage` trait picking the `stack_blob` / `heap_blob` / `shallow_blob` backend, `data()`, and
the `kumi::identifier` interface. The per-point `element` and `scheme` machinery it hosts live in
the sibling headers `include/kiwaku/memory/elements.hpp` (the `element` stalk) and
`include/kiwaku/memory/scheme.hpp` (the `scheme` sheaf / per-field offsets), assembled above the
blob in `kwk::__`.

- `blob<Storage>` is the single arity; the concrete backend (`total_space`) is
  `kwk::__::as_storage_t<Storage>`. The protocol adapters are consistent with this: because a blob
  is an **opaque raw byte region** (not a product type), **no** `tuple_size`/`tuple_element` are
  provided for it — the per-field views belong to the `\ref kwk_arch_scheme "scheme"` layer above,
  not to the blob. (An earlier "arity mismatch" concern was resolved by dropping the two-parameter
  product specializations rather than complicating the backend.)
- `blob::swap` forwards to `total_space::swap` on the chosen backend, which properly exchanges both
  members — it is **not** lossy; this was a stale reading of an old draft. Ownership move/assign and
  swap behave as usual for owning backends.

**Relationship to the sheaf core.** The `element` (a `kind` plus a byte offset) and `scheme` (the
per-point offset map, a kumi record) types live above the blob in the `kwk::__` namespace — the
`element` in `memory/elements.hpp`, the `scheme` in `memory/scheme.hpp` — and compile cleanly
without pulling in the blob backend; `memory.hpp` (→ `kiwaku.hpp`) assembles them all without issue.

Continue to \ref kwk_arch_element "the point".
