#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_quickstart Quickstart

@section kwk_quickstart_first A first container

Include KIWAKU and, for convenience, its compile-time extent literals:

@code
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;
@endcode

A *view* borrows memory you already own; a *table* owns a fresh region. Start with a view over a
plain C array:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float  data[6] = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
auto   a      = kwk::view(kwk::source = data, kwk::shape{2_c, 3_c});   // a 2x3 view over `data`
```

`a` is a 2-dimensional collection over `data`. Its shape and size are introspectable:

```cpp
a.ndim;          // 2, a compile-time constant
a->shape();      // {2, 3}
a->size();       // 6 (total element count)
```

Reading elements uses the same syntax in any dimension — scalars for a full position, a slice
or sub-view otherwise:

```cpp
a[0, 0]      == 1.f;    // positional read, row 0 column 0
a[1, 2]      == 6.f;    // positional read, row 1 column 2
a[0]         ;          // partial index -> a borrowed 1x3 sub-view (tile), not a scalar
a[0][0, 2]   == 3.f;    // index into the tile
```

Writing is symmetric:

```cpp
a[0, 0] = 9.f;          // writes through the view into `data`
data[0]  ;              // now == 9.f
```

@section kwk_quickstart_operations Operating on it

Slicing returns a borrowed sub-view that aliases the parent — like a NumPy view, not a copy:

```cpp
using kwk::_;
auto row = a[1];                        // the second row, aliasing `data`
auto mid = a[kwk::slicer{0, 2, 1}];     // first two columns, still aliasing `data`
```

Out-of-domain and fractional reads are governed by boundary conditions and interpolators:

```cpp
auto v = kwk::view(kwk::source = data, kwk::shape{6_c});
auto t = kwk::view(kwk::source = data, kwk::shape{6_c}, kwk::tile);     // periodic
auto l = kwk::view(kwk::source = data, kwk::shape{6_c}, kwk::linear);  // interpolate
l[2.5];     // linear blend of l[2] and l[3]
```

Whole-collection algorithms iterate the shape for you:

```cpp
kwk::fill(a, 0.0f);        // every cell of `a` becomes 0
kwk::copy( dst, src );     // dst[i..] = src[i..] element-wise
```

Function-defined spaces evaluate on demand and never allocate:

```cpp
constexpr auto sum = [](auto... xs) { return (xs + ...); };
auto g = kwk::table(kwk::generator = sum, kwk::shape{3_c, 4_c});
g[1, 2];     // == 3 (coordinates summed)
```

For a structured (struct-of-arrays) container whose fields you address by name-positioning,
see `kwk::as<K>` in the fundamentals pages.

@section kwk_quickstart_next Where to go next

- @subpage kwk_beginners — the absolute basics: containers, shapes, ownership.
- @subpage kwk_creation — all the ways to build a container.
- @subpage kwk_indexing and @subpage kwk_slicing — reading and re-shaping data.
- @subpage kwk_copies — when a view, slice, or table copies and when it does not.
- The @subpage kwk_fundamentals hub for the full topic list.

**/