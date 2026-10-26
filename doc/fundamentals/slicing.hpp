#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_slicing Slicing mechanism

Slicing cuts a **range** along one or more dimensions and returns a *borrowed sub-view* that
**aliases** the parent — it is a view into the same memory, not a copy (NumPy view semantics).
This page covers the `a[start:stop:step]` analog; scalar and tile indexing are on
@subpage kwk_indexing.

@section kwk_slicing_index Slicing indexes along a dimension

A `kwk::slicer` describes a range of positions along one dimension:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float  buf[7] = {1, 2, 3, 4, 5, 6, 7};
auto   v      = kwk::view(kwk::source = buf, kwk::shape{7_c});

v[kwk::slicer{2, 6, 2}];   // positions 2, 4      (start inclusive, stop exclusive, step 2)
v[kwk::slicer{0, 7, 1}];   // all 7 positions
```

The three fields map onto a NumPy slice as:

| `kwk::slicer{start, stop, step}` | `a[start:stop:step]` |
|----------------------------------|----------------------|
| start                            | start index (inclusive) |
| stop                             | end index (exclusive)   |
| step                             | stride / increment      |

Any field may be a *wildcard*, spelled `k` (from `using kwk::_`), which stands for the safe
default — `0` for start, `kwk::end` for stop, `1` for step:

```cpp
using kwk::_;
v[kwk::slicer{_, 6, 2}];   // from position 0   -> positions 0, 2, 4
v[kwk::slicer{2, _, 1}];   // to the end        -> positions 2..6
v[kwk::slicer{2, 6, _}];   // step 1            -> positions 2, 3, 4, 5
v[kwk::slicer{_, _, _}];   // the whole range (same as a[ _ ])
```

`kwk::end` is the sentinel meaning "the last position"; a derived end such as `kwk::end / 2`
selects a fraction of the extent. To turn a bare index or a wildcard into a slicer, use
`kwk::to_slicer`:

```cpp
auto all      = kwk::to_slicer(_);     // the whole dimension
auto from_two = kwk::to_slicer(2);     // positions 2..end
v[from_two];
```

`kwk::to_slicer` is exactly what `operator[]` calls on its arguments, so you can pass any slicer
directly (an integer, a `kwk::slicer`, a wildcard) to `v[...]`.

@section kwk_slicing_views Slicing entire views

Pass one slicer **per dimension** to slice a multi-dimensional container; use `_` for the
dimensions you keep whole:

```cpp
float   grid[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
auto    a        = kwk::view(kwk::source = grid, kwk::shape{3_c, 4_c});

using kwk::_;
a[_ , kwk::slicer{0, 2, 1}];   // columns 0..1 of every row   (a 3x2 view)
a[_ , kwk::slicer{0, 4, 2}];   // columns 0 and 2             (a 3x2 view)
a[1 , _];                      // only row 1                  (a 1x4 view)
```

Every result is a **borrowed sub-view that aliases the parent**. Writing through it is visible in
the parent and in any other view, and the parent storage is untouched otherwise:

```cpp
auto cols = a[_ , kwk::slicer{0, 2, 1}];
cols[0, 0] = 99;               // == a[0, 0]  (same cell via alias)
```

Slicing composes with product (struct-of-arrays) kinds: the sub-view re-bases **each** field
fibre to the sliced offset, so `kwk::get<I>` on the slice sees the right records:

```cpp
using K = kumi::tuple<float, double>;

float       fs[4] = {1.f, 2.f, 3.f, 4.f};
double      ds[4] = {10., 20., 30., 40.};

auto s      = kwk::table(kwk::source = kumi::tuple{fs, ds}, kwk::shape{4_c}, kwk::as<K>());
auto sl     = s[kwk::slicer{2, 4, 1}];     // records 2..3
kwk::get<0>(sl)[0];                        // == 3.f (slice of the float  fibre)
kwk::get<1>(sl)[0];                        // == 30. (slice of the double fibre)
```

The slice keeps borrowing the parent's heap region (it re-points to `data()` and re-bases each
fibre), so slicing an owning `table` does **not** copy.

@section kwk_slicing_custom Custom slicer definition

Any type can be used as a slicer if it models the concept `kwk::concepts::slicer` — that is, if a
free function `to_slicer(t)` is findable for it (via argument-dependent lookup). `kwk::slicer`,
an integer, and `kwk::fixed<1>` all model it, which is why `to_slicer(2)`, `to_slicer(2_c)`, and
`to_slicer(_)` work.

To plug in a custom range type, overload `to_slicer` in your own namespace (found by ADL) and
return a `kwk::slicer` describing the positions:

```cpp
namespace my
{
  struct every_third { std::size_t offset; };

  // ADL-findable: makes `every_third` a valid slicer for kwk collections.
  auto to_slicer(every_third e) { return kwk::slicer{e.offset, kwk::end, 3}; }
}

auto e = my::every_third{1};
v[e];           // positions 1, 4, 7  ...
```

The returned `kwk::slicer` has a small iterator interface (`start()`/`stop()`/`step()`), which is
how the slicing access walks the selected positions.

**/
