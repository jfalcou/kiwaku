# The Sheaf: scheme
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_scheme The Sheaf: *scheme*

# The Sheaf: `scheme`

## What it is

`kwk::__::scheme<T, Layout...>` is the object that **organizes `element`s over a shared
`\ref kwk_arch_blob "blob"`**. Where a `blob` is "there is memory of this size," the scheme is
"and here is *how* that memory is carved into typed points — which `kind` sits where." It is the
semantic heart of the representation: two schemes over the same blob differ precisely in *what the
blob means*.

Its central discriminator is a compile-time boolean,

\code{.cpp}
static constexpr bool is_product_space = kumi::concepts::product_type<T>;
\endcode

which selects between the two fundamental geometries:

| `is_product_space` | meaning | storage geometry |
|---|---|---|
| `false` | a single homogeneous `kind` fills the space | trivial / uniform |
| `true`  | the `kind` is a product type; each field is its own array | **structure-of-arrays (SoA)** |

## What it is mathematically

Recall from \ref kwk_arch_foundations that a **sheaf** assigns to every open region a set of
sections and glues compatible local data uniquely. The scheme is the layer that turns the bare base
\f$B\f$ into a **sheaf / fibered structure**:

- In **uniform mode** (`is_product_space == false`) every fiber is the single fixed type \f$T\f$;
  this is the trivial fiber bundle \f$B\times T \to B\f$.
- In **product / SoA mode** (`is_product_space == true`) the field \f$T \simeq T_0\times\cdots\times T_{n-1}\f$
  is split into one fiber per component \f$T_i\f$, each with its own offset: a genuine multi-fibered
  (Grothendieck-style fibered) structure, and the storage is **structure-of-arrays** — one flat array
  per component.

This is exactly the distinction flagged in `collection.hpp`:

> "If i am a sheaf (fields of different sizes): Iliffe vector/jagged stuff.
>  If i am a fiber bundle (fields of the same sizes): Standard SoA."

So `scheme` encodes which of the two Grothendieck regimes the collection lives in, and, in the
bundle/sheaf reading, *glues* the per-field arrays into a single shared space.

## What it does

- Binds a `kind` (the fiber type \f$T\f$) and a `Layout` (the per-field geometry) to a shared blob.
- In **uniform mode**: exposes `data(blob_base)` as the field pointer, and `operator[]` as
  `reinterpret_cast<T*>(data)[index]` — a flat, strictly positive array.
- In **product mode**: `data(begin)` returns a `kumi::tuple{ begin, begin + off_1, ... }`, one pointer
  per field; `get<I>` and `operator[]` then view each field's own SoA array, while `data()` provides
  the coalesced view.
- Supplies the tuple/record protocol (`get<I>`, `operator<<`) so a scheme itself behaves like a
  kumi product and can be composed.

## How to implement it

**Status: implemented and compiling.** In `include/kiwaku/memory/scheme.hpp` the `scheme` is a record
type built on the compressed union of its per-field offsets:

\code{.cpp}
template<typename Kind, std::size_t... Offset> struct scheme : compressed_tuple<Offset...> { ... };
\endcode

- It lives in `kwk::__`, is a **kumi record** (`is_record_type< scheme<...> >` is `true_type`), and
  its per-field offsets are read through the base's `get<I>` or via the hosting blob's `data()` and
  `operator[]`.
- A blob is an opaque byte region (no product protocol itself); the **scheme** is the layer that
  carves that blob into typed points and fields. Its hosting object resolves `(index, field)` down to
  an `\ref kwk_arch_element "element"` byte offset within the shared `data()`.

**Current and future split.** Today the scheme implements the *fiber-bundle* geometry: one shared
shape and one uniform layout, with per-field byte offsets giving the SoA arrays. The genuinely
heterogeneous direction — each field with its *own* shape/extents (jagged, Iliffe-style) — is
**Phase K** (see \ref kwk_finalization_roadmap "the finalization roadmap"): the shared-shape uniform
case collapses to the byte-identical SoA fast path, while differing field shapes get per-field
geometry. The `is_product_space` split and the `data(begin)` contract already anticipated exactly
this dual regime and remain the semantics in force. (These pages call the heterogeneous case "the
sheaf" loosely; precisely it is a *fibered / dependent-container* structure — see the precision note
in \ref kwk_arch_collection "the containers".)

Continue to \ref kwk_arch_kind "the type".
