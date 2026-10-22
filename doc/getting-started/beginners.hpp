#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_beginners Absolute basics for beginners

@section kwk_beginners_container What a container holds

A KIWAKU **collection** (built as either a `kwk::table` or a `kwk::view`) is one object that
describes:

- **what** it stores — the *element kind* (e.g. `float`, `unsigned`, or a tuple of fields);
- **how many** and **in how many dimensions** — its *shape* (a possibly nested list of extents);
- **where** those cells live in memory — its *stride* / layout;
- **how a logical position maps to a cell** — its *accessor* (coordinates, tiling, boundaries,
  interpolation).

Reading and writing go through one uniform, positional syntax; you do not manage the indices by
hand. The collection is the single object you pass around and index.

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float  grid[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
auto   a       = kwk::view(kwk::source = grid, kwk::shape{3_c, 4_c});   // a 3x4 collection
```

`a` is a 2-dimensional collection of `float` cells. Its element kind, dimension count, shape,
and total size are all available:

```cpp
a->kind();       // kwk::as<float>()  - the element kind
a.ndim;          // 2               - compile-time number of dimensions
a->shape();      // {3, 4}          - per-axis extents
a->size();       // 12              - total number of cells
```

@section kwk_beginners_shape Shapes and extents

A shape is a collection of extents, one per dimension. Runtime (dynamic) extents are written as
plain integers; compile-time (static) extents use the ``_c`` suffix from `kwk::literals`:

```cpp
kwk::shape{3, 4};            // 3x4, both extents dynamic
kwk::shape{3_c, 4_c};        // 3x4, both extents known at compile time
kwk::shape{3_c, 4};          // mixed: rows static, columns dynamic
```

Dimensionality is encoded in the *type*. A `kwk::shape{4_c}` is a 1-dimensional space;
`kwk::shape{2_c, 3_c}` is 2-dimensional; `kwk::shape{3_c, 3_c, 3_c}` is 3-dimensional. Shapes may
also be **nested** — `kwk::shape{3_c, kwk::shape{2_c, 2_c}}` is a grid of three 2x2 tiles — which
changes how partial indexing behaves (see @subpage kwk_indexing).

The total cell count is `->size()`; the per-axis extents come from `->shape()`.

@section kwk_beginners_access Reading and writing elements

Elements are read and written positionally. Use one index per dimension for a scalar cell:

```cpp
a[0, 0];             // read cell at row 0, column 0
a[2, 3];             // read cell at row 2, column 3  (== 11)
a[1, 2] = 42.f;      // write: updates the underlying memory
grid[6];             // now == 42.f (the view aliases the array)
```

When you index with *fewer* indices than there are dimensions you get a **sub-collection**
(tile) back, not a scalar — exactly how NumPy returns a smaller array:

```cpp
auto first_row = a[0];          // a borrowed 1x4 sub-view over row 0
first_row[0, 3] == grid[3];     // reads through into the parent
```

Slices (a range along one or more dimensions) also return borrowed, aliasing sub-views:

```cpp
using kwk::_;
auto cols01 = a[_ , kwk::slicer{0, 2, 1}];   // columns 0..1 of every row - aliases `grid`
auto step  = a[_ , kwk::slicer{0, 4, 2}];    // columns 0 and 2
```

Whether an index is a scalar read, a tile, or a slice is decided by the operands you pass to
`operator[]`, and every result that is a sub-view shares the parent's memory (writing through it
is visible in the parent).

@section kwk_beginners_ownership Owning and borrowing data

The single most important distinction is **who owns the memory**:

- A **`kwk::view` borrows** memory you already own. It never allocates and never frees. It is the
  natural way to wrap a buffer you already hold:

  ```cpp
  float  buf[8] = {0,1,2,3,4,5,6,7};
  auto   v     = kwk::view(kwk::source = buf, kwk::shape{8_c});
  ```

- A **`kwk::table` owns** a fresh region. When you build it from a source, the data is copied
  into newly owned storage:

  ```cpp
  auto  t = kwk::table(kwk::source = buf, kwk::shape{8_c});   // owns its own copy
  ```

Wrapping a `const` source makes a view read-only at *compile time*: any attempt to write through
it is a compile error, not a runtime fault.

```cpp
float const  cbuf[8] = {0,1,2,3,4,5,6,7};
auto         c       = kwk::view(kwk::source = cbuf, kwk::shape{8_c});
// c[0] = 1.f;  // does not compile - a const source is read-only
```

Objects you get back from slicing or partial indexing — whether over a `view` or a `table` — are
borrowed sub-views: they alias the object they were cut from and keep no ownership of their own.
This is NumPy-view semantics made explicit in the type, so sharing memory can never surprise you
at run time.

A collection can also hold **no memory at all**: a generator-backed `table` evaluates every cell
on demand from its grid coordinates (see @subpage kwk_creation).

**/