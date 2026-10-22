# Architecture

<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_architecture Architecture

# Architecture {#architecture}

KIWAKU is a header-only, C++23 library whose goal is the **most general, non-self-referential,
mathematically rigorous representation of collection types**. It does not hand-write recursive data
structures (lists, grids, N-D arrays as bespoke classes). Instead, every collection is **described
declaratively** as a combination of typed settings, and the actual storage type is derived from that
description. The design is deliberately modeled on **Grothendieck-style constructions**: sheaves,
fibered structures, descent, and the categorical theory of *containers* (shape-and-positions
representations of strictly positive types).

These pages describe each building block under four headings:

1. **What it is** — the concrete C++ object.
2. **What it is mathematically** — the Grothendieck / category-theoretic / type-theoretic notion it realizes.
3. **What it does** — behavior and responsibilities.
4. **How to implement it** — the concrete implementation, what exists, what is open.

## Layout of the space

```
     collection / table / view  ......... the user-facing globals (a *global section*)
                          │
              options ── builder ──────── the specification & *descent* (local specs → global object)
                          │
            layout (shape × stride)  .... the *fiber*: where each field lives over the base
                          │
                  accessor  .............. the *topology*: how a logical point is reached (coordinate, boundary, interpolation, tiling)
                          │
              scheme ──organizes──► element  ... the *sheaf*: shared space, per-point stalks
                          │
                      blob  ............... the *total space / base object*: raw memory
```

The role each term plays in the mathematics:

| Kiwaku | Grothendieck / categorical notion |
|---|---|
| `blob` | base space / total space object `E` (raw memory) |
| `element` | a **stalk** / **germ**: the datum at a single point |
| `scheme` | the **sheaf** (gluing of stalks over a shared space), echoing Grothendieck *schemes* as locally structured spaces |
| `kind` | the **strictly positive (container) type**: the fiber type `X` |
| `shape` / `stride` | the **positions** `P s` and their embedding in the base |
| `layout` (shape × stride) | the **fiber** \f$F\f$ / local trivialization \f$U\times F\f$ |
| `accessor` | the **topology / section-resolution** (restriction, glueing, boundary) |
| `options` + `builder` | **descent**: assembling a global object from compatible local specifications |
| `collection` / `table` / `view` | the **total object / global section** obtained by gluing |

## Reading order

- \subpage kwk_arch_foundations
- \subpage kwk_arch_blob
- \subpage kwk_arch_element
- \subpage kwk_arch_scheme
- \subpage kwk_arch_kind
- \subpage kwk_arch_shape_position
- \subpage kwk_arch_layout
- \subpage kwk_arch_accessor
- \subpage kwk_arch_options
- \subpage kwk_arch_collection

The chapters above are ordered pedagogically, ground-up from the base (blob → element → scheme → …).
The *implementation* dependency order is the reverse build-up — scheme on element, blob hosting
scheme, layout on shape/stride, accessor on layout, collection on all of them
(element → scheme → blob → layout → accessor → collection). All of these layers are implemented,
tested and wired together. What remains — chiefly per-field (sheaf/jagged) geometry — is tracked in
the separate **\ref kwk_finalization_roadmap "Finalization Roadmap"**.
