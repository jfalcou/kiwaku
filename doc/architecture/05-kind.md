# The Type: kind
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_kind The Type: *kind*

# The Type: `kind`

## What it is

`kind` is the option that names the **type of the elements** of a collection. In code it is written
with the identifier `kwk::kind` and the dedicated `as<T>()` helper,

\code{.cpp}
kwk::table(kwk::shape = 4_c, kwk::kind = kwk::as<double>());   // a 4-vector of double
kwk::kind = kwk::int32;                                        // shorthand for as<std::int32_t>()
\endcode

The stored object is `kwk::__::kind_option<T>`, carrying `element_type = T`,
`itemsize = sizeof(T)`, and a `kind` alias. Ready-made `real32`, `real64`, `int8` … `uint64` are
provided. When the `kind` is a product type (a `kumi` record), its components are the "fields" of the
collection and — combined with `\ref kwk_arch_scheme "scheme"` — select the SoA layout.

The concrete per-component types are the **outer** members of the product — the fields addressed by
`kumi::element_t<I, T>` for `I < kumi::size_v<T>`, *not* `kumi::result::flatten_all` — which the
builder walks field-by-field when synthesizing storage and offsets. SoA therefore splits the product
at its **outer** boundary only: a nested (record-typed) field is laid out as one interleaved fiber
of its whole struct type, and is *not* sub-split into its leaves.

## What it is mathematically

`kind` is the **fiber type** \f$X\f$ of the whole construction — the strictly positive type the
collection is "made of" (see \ref kwk_arch_foundations). In the container reading
\f$\llbracket S\triangleright P\rrbracket(X) = \sum_{s:S} \prod_{p:P(s)} X,\f$

`kind` supplies exactly the \f$X\f$: the datum placed into every position. The **shape** \f$S\f$
and **positions** \f$P\f$ (`\ref kwk_arch_shape_position`) are orthogonal to it — a fact the
container functor makes precise: \f$X\f$ is varied/to-be-filled.

When `kind` is a **product type** \f$X = X_0\times\cdots\times X_{n-1}\f$, the container theorem
gives two dual representations:

- **array-of-slices (AoS)** — store \f$(X_0,\dots,X_{n-1})\f$ contiguously per position;
- **structure-of-arrays (SoA)** — store one array per \f$X_i\f$.

Kiwaku's uniform-vs-sheaf split (`\ref kwk_arch_scheme`) realizes the **SoA / fibered** side:
the disjoint union of the per-field types \f$X_0\oplus\cdots\oplus X_{n-1}\f$ — one fiber per *outer*
field, each fiber itself possibly a whole struct — is exactly the "one array per field, stacked or
offset in one blob" description. This is the point
where the categorical "strictly positive" story meets the layout engine.

## What it does

- Names the element type \f$T\f$ and lets the builder compute storage (`sizeof`, alignment,
  per-field flattening).
- Distinguishes scalar vs product (record) kinds; the product case activates the SoA/fielded regime.
- Participates in the `options` bag as a kumi field, so it can be defaulted, probed with `get`,
  and checked for congruence with the shape.
- Is checked against the shape by the builder (one shared shape today; per-field congruence is
  **Phase K**, replacing the historical `congruent` placeholder).

## How to implement it

**Status: complete and compiling.** `include/kiwaku/settings/kind.hpp` implements `kind_option<T>`,
the `kind` identifier, `as<T>()`, and the standard scalar aliases, with a clean kumi-field protocol
(uniquely named, labeled, printable, equality-tagged).

**Notes and open work.**

1. **The builder's product-kind path is done.** `builder_t` computes per-field storage
   (`static_owning_bytes`) and produces per-field byte offsets via `make_offsets` — that is, the
   `scheme`'s `offsets` — so a product `kind` already realizes an SoA scheme.
2. **The "shape-per-component" generalization.** The richer contract is described in the builder:
   *"Kind becomes a smarter thing: for each component, store an ID which is the shape that
   corresponds."* That is the *sheaf* regime — different fields with different shapes — versus the
   fiber-bundle regime (one shared shape). It is design intent, to be realized in **Phase K**
   (see \ref kwk_finalization_roadmap "the finalization roadmap"), alongside the real per-field
   `congruent` check.
3. **`kind` stays out of "self-reference."** Because `kind` is only ever a (strictly positive) type
   used as \f$X\f$ and never recurses through the container's own structure, it is the type-theoretic
   guarantee behind the "non-self-referential" claim: the recursion, if any, lives in the *positions*
   (\f$W\f$-type / container fixpoint), never in `kind` itself.

Continue to \ref kwk_arch_shape_position "positions: shape & stride".
