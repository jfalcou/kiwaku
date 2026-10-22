# Mathematical Foundations
<!--
  KIWAKU - Shape Up Your Data
  Copyright : KIWAKU Project Contributors
  SPDX-License-Identifier: BSL-1.0
-->
\page kwk_arch_foundations Mathematical Foundations

# Mathematical Foundations

This page collects the vocabulary used across the architecture pages. KIWAKU is *not* a
re-implementation of algebraic geometry; rather it takes Grothendieck's way of structuring
"the data of a space" as the organizing principle for container design. Each claim below is a
standard result; pointers to sources are given at the end.

## Categories and functors

A **category** \f$\mathcal{C}\f$ has **objects** and **morphisms** between them, closed under
identity and composition. A **functor** \f$F : \mathcal{C}\to\mathcal{D}\f$ maps objects to
objects and morphisms to morphisms, preserving composition. In what follows the relevant
categories are usually the category of (small) sets, of types, or of "data shapes."

## Strictly positive types and containers

A **strictly positive type** is a type expression built only from:

- constant types \f$K\f$,
- type variables \f$X_i\f$,
- **sums** \f$U+V\f$ and **products** \f$U\times V\f$,
- constant exponentials \f$K\Rightarrow U\f$,
- (optionally) least/greatest fixed points \f$\mu X.U\f$, \f$\nu X.U\f$.

It is a type that, *at the level of the outer shape*, does not recursively **refer to itself** —
this is the type-theoretic meaning of KIWAKU's "a collection type that does not refer to itself."
A **container** is the canonical representation of such types as a pair of a **shape** set \f$S\f$
and an \f$S\f$-indexed family of **position** sets \f$P\f$, whose interpretation as a functor is

\f$\llbracket S\triangleright P\rrbracket(X) \;=\; \sum_{s:S}\,\prod_{p:P(s)} X \;=\; \{\,(s,\,f)\mid s:S,\ f:P(s)\to X\,\}.\f$

A shape \f$s\f$ selects "how many slots", the family \f$P(s)\f$ names the slots, and each slot is
filled with an \f$X\f$. Lists, arrays, matrices, trees and higher arrays all arise this way
(Abbott–Altenkirch–Ghani, *Containers: Constructing Strictly Positive Types*).

This is the formal home of Kiwaku's `shape ∘ kind`: the *positions* are the index set, the *fiber
type* \f$X\f$ is the `kind`, and the storage size is the cardinal of the product of extents.

## Fiber bundles

A **fiber bundle** \f$F \to E \overset{\pi}{\to} B\f$ is a continuous surjective **projection**
\f$\pi\f$ from a **total space** \f$E\f$ onto a **base space** \f$B\f$, such that every base point
has a neighborhood \f$U\f$ with a homeomorphism (**local trivialization**)
\f$\pi^{-1}(U)\ \cong\ U\times F\f$

making the obvious triangle commute. \f$F\f$ is the **fiber**; all fibers look like \f$F\f$, but the
bundle may be globally *twisted* (the Möbius band over the circle is the canonical example). A
**section** is a map \f$\sigma : B \to E\f$ with \f$\pi\circ\sigma = \mathrm{id}_B\f$ — a coherent choice
of one element of the fiber above every base point.

Grothendieck generalized bundles into **fibered categories**: a family of objects varying over a base,
where the projection is a (cartesian) functor. A bundle whose fiber is a vector space is the familiar
**vector bundle**; a bundle that is globally \f$B\times F\f$ is **trivial**.

## Presheaves and sheaves

A **presheaf** on a space/site \f$X\f$ assigns to every open set \f$U \subseteq X\f$ a set of
**sections** \f$F(U)\f$, together with **restriction** maps \f$-\restriction_V : F(U)\to F(V)\f$ for
\f$V\subseteq U\f$ (a contravariant functor on the poset/category of open sets).

A **sheaf** is a presheaf whose compatible local sections **glue** uniquely: given an open cover
\f$\{U_i\}\f$ of \f$U\f$,

1. (locality) two sections agreeing on every \f$U_i\f$ are equal;
2. (gluing / descent) local sections agreeing on overlaps \f$U_i\cap U_j\f$ assemble into a unique
   global section over \f$U\f$.

The data "visible near a point \f$p\f$" — the **stalk** — is the direct limit over neighborhoods of
\f$p\f$,
\f$F_p \;=\; \varinjlim_{p\,\in\,U}\, F(U),\f$

and its elements are **germs**. A **global section** is a coherent choice of one germ per point, i.e.
an element of \f$F(X)\f$. (nLab *sheaf*; Hartshorne, *Algebraic Geometry*, ch. II.)

In Grothendieck's language the "open sets + covers" structure is axiomatized as a **site** with a
**Grothendieck topology**; the sheaf condition becomes **descent** along covering families. The
**Grothendieck construction** \f$\textstyle\int F\f$ (the category of elements) assembles a presheaf
\f$F\f$ into a fibered category whose objects are pairs \f$(U,\,x)\f$ with \f$x\in F(U)\f$ — 
"an open set together with a local datum over it."

## How the two meet (Kiwaku's stance)

A **fiber bundle** has *one fixed fiber type* over the whole base; a **sheaf** lets the data over
points be *finer and glued*. Kiwaku uses both:

- **uniform / fiber-bundle mode** — every field shares the same shape/fiber; storage is
  structure-of-arrays (SoA), the trivial bundle \f$B\times F\to B\f$;
- **sheaf mode** (heterogeneous) — fields have *different* shapes and sizes; storage is jagged /
  Iliffe-orientation, a genuine sheaf whose local data must be glued coherently.

The **accessor** plays the role of the topology/section-resolving machinery; the **builder** plays the
role of descent — assembling a global collection from compatible local specifications.

## References

- A. Grothendieck, *Sur quelques points d'algèbre homologique* (Tohoku, 1957) — the origin of abelian
  categories, sheaf cohomology, and the "heyday" of treating a space through the sheaves on it.
- R. Hartshorne, *Algebraic Geometry*, Springer GTM 52, 1977, ch. II — sheaves, schemes, stalks, glueing.
- nLab, *[sheaf](https://ncatlab.org/nlab/show/sheaf)*, *[presheaf](https://ncatlab.org/nlab/show/presheaf)*,
  *[stalk](https://ncatlab.org/nlab/show/stalk)*, *[Grothendieck topology](https://ncatlab.org/nlab/show/Grothendieck+topology)*,
  *[fibered category](https://ncatlab.org/nlab/show/fibered+category)*.
- Wikipedia, *[Fibre bundle](https://en.wikipedia.org/wiki/Fibre_bundle)*.
- M. Abbott, T. Altenkirch, N. Ghani, *Containers: Constructing strictly positive types*, Theor. Comput.
  Sci. 342 (2005) 3–27 — shape-and-positions representation of datatypes, and its universal (initial
  algebra / \f$W\f$-type) characterization.

Continue to \ref kwk_arch_blob "the total space".
