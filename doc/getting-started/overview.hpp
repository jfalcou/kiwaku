#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_overview What KIWAKU is

@section kwk_overview_purpose Purpose

KIWAKU is a **header-only C++23 library for n-dimensional collections**. It is the C++ answer
to the class of problems Python solves with NumPy: describing, creating, indexing, slicing, and
shaping multidimensional data — except that it leans on the C++ type system to get zero-cost,
non-owning views, correct-by-construction memory handling, and compile-time knowledge of shape,
layout, and element kind wherever they are known.

The unifying idea is the **collection**: a type that brings together *what a collection is*
(storage), *how it is laid out* (shape × stride), and *how a logical point is addressed*
(coordinates, tiling, boundary conditions, interpolation) into a single object you read and
write through a uniform, NumPy-like position syntax:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float  cube[8] = {0,1,2,3,4,5,6,7};
auto   a      = kwk::view(kwk::source = cube, kwk::shape{2_c, 2_c, 2_c});

a[0,1,1]            // positional scalar read  (== 3)
a[0]                // partial index: a *tiled* sub-view (a 2x2 collection, not a scalar)
a[kwk::slicer{..}]  // slice -> a borrowed sub-view aliasing `cube`
```

The core design principle is **non-self-referential description**: no recursive "array of
arrays" classes. Instead every collection is described declaratively from typed settings
(storage, shape, stride, accessor, kind), and the concrete type is derived from that
description — the same philosophy expressed in `doc/architecture/*.md`, where these pieces are
grounded in Grothendieck-style constructions (fibered structures and sheaf-theoretic glueing of
local specifications into a global object).

@section kwk_overview_scope Scope

KIWAKU provides, out of the box and backed by its unit tests:

- **Creating** collections over existing memory (`kwk::view`, borrowing) or as fresh owners
  (`kwk::table`), for plain scalar kinds and for **product (struct-of-arrays) kinds** via
  `kwk::as<kumi::tuple<...>>()`.
- **Shape** — runtime and compile-time (`_c` suffix) extents, and nested (tiled) shapes.
- **Addressing** — scalar positional reads/writes (`a[i]`, `a[i,j]`), partial indexing that
  yields tiled sub-collections, and slicing (`kwk::slicer`, `kwk::end`) that yields borrowed,
  aliasing sub-views.
- **Boundary conditions** (`kwk::clamp`, `kwk::tile`, `kwk::mirror`) and **interpolation**
  (`kwk::linear`, `kwk::nearest`, `kwk::floor`, `kwk::ceil`, `kwk::round`) for reading at or
  beyond the domain with non-integer coordinates.
- **Algorithms** — `kwk::copy`, `kwk::fill`, `kwk::initialize`, `kwk::destroy`.
- **Whole-array, storage-less spaces** via generators (`kwk::table(kwk::generator = f, ...)`)
  that evaluate on demand from grid coordinates and never allocate.

Deliberately **out of scope today** (callouts for what is not implemented, rather than hidden):
universal-function (`ufunc`) / reduction machinery, broadcasting, `reshape`/`stack`-style
layout surgery, fancy/boolean/mesh indexing, and file/stream I/O. These are absent by design in
this phase; the relevant fundamentals pages call out the supported alternative for each. A
running ledger of implemented vs. deferred features lives in `kiwaku/direction-DONE.md`.

KIWAKU is **header-only** and **C++23**, installable via CPM or as a fetched package, and is
written to compile cleanly under `-Wall -Wextra -Werror -Wdocumentation` with no Doxygen
warnings.

**/