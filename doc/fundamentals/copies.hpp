#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_copies Copies and views

In NumPy, "views" are new array objects that reference the same data, and "copies" are
new arrays that own their own data. KIWAKU makes the same distinction at the type level:
a collection either **borrows** caller-provided memory (a `view`) or **owns** a fresh
region (`table`), and every slice/tile of either is itself a *borrowing view* that
aliases the parent. This page walks the no-copy / shallow-copy / deep-copy spectrum.

@section kwk_copies_none No copy at all

**A `view` never copies and never allocates.** It shallow-wraps a caller-provided source
(a C array, `std::array`, a contiguous range, or a raw pointer) and adds shape, stride
and accessor — its size is a couple of pointers and a descriptor, independent of the
data:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float buffer[6] = {0, 1, 2, 3, 4, 5};
auto  v        = kwk::view(kwk::source = buffer, kwk::shape{6_c});
```

Reading and writing go straight to `buffer`. Because `v` aliases the caller's memory, any
write through `v` (or any of its sub-views) is immediately visible to whoever owns
`buffer` — there is no divergent copy state.

Every indexed sub-view is also *no-copy*: slicing a `view` and partial (tile) indexing
both produce a new borrowed descriptor over the same bytes:

```cpp
auto s      = v[kwk::slicer{2, 5, 1}];   // a view over buffer[2..5], no copy
auto single = kwk::get<0>(product_view); // a per-field fibre view, no copy
```

**Slices and tiles alias the parent.** Modifying a slice modifies the parent and every
other view of the same memory. This is NumPy-view semantics, but enforced by
construction rather than by convention — there is no way to slice a collection and get a
copy unless you explicitly ask for an owning result.

@section kwk_copies_shallow View or shallow copy

Because a `view`/collection is a lightweight value type, *copying the value* is a
shallow copy: the two collection objects share the same descriptor pointers and address
the same memory.

```cpp
auto v  = kwk::view(kwk::source = buffer, kwk::shape{6_c});
auto w  = v;            // shallow copy: same memory, same extents descriptor
w[0] = 42;              // visible through v (and buffer)
```

There is **no reference-counted / copy-on-write ownership** in the library (there is no
`book`-style shared handle). Instead, ownership is an either/or at the factory: a
`view` borrows, a `table` owns. So the two meaningful choices are the no-copy route
above (borrow) and the deep-copy route below (own).

**An owning `table` is move-only.** A dynamic (heap) `table` owns its region through a
move-only handle, so copying the *table value* does not compile — you cannot
accidentally share or duplicate an owning region by value:

```cpp
auto t = kwk::table(kwk::source = buffer, kwk::shape{n});   // owns a heap region (n runtime)
// auto u = t;   // does NOT compile: the owning handle is move-only
```

To read the same owned data through a shareable, non-owning handle, take `t` as a
borrowed view or index it directly.

@section kwk_copies_deep Deep copy

There are two ways to get a genuinely independent, owning copy of some data.

**At construction** — `kwk::table` with a source allocates a fresh region and copies the
source in, so the `table` no longer aliases its input:

```cpp
float  buffer[4] = {1, 2, 3, 4};
auto   t         = kwk::table(kwk::source = buffer, kwk::shape{4_c});   // owns its own copy

t[0] = 99;        // buffer[0] is untouched (still 1)
```

A static shape owns inline storage (no heap) and a runtime shape owns a heap region, but
in both cases the result is a fresh, independent allocation — the NumPy `.copy()`
analogue.

**After construction** — `kwk::copy` copies every cell of one collection into the
corresponding cell of another, element-wise over the shape:

```cpp
auto src = kwk::view(kwk::source = buffer, kwk::shape{4_c});
auto dst = kwk::table(kwk::shape{4_c});   // owning destination, same shape
kwk::copy(dst, src);                      // dst[i] = src[i] for every i
```

`kwk::copy` is a whole-collection algorithm (it lives with the other memory mutators
described on @subpage kwk_universal) and also works on sub-views and
mismatched-but-compatible layouts, since it assigns by position (`out[idxs...] =
in[idxs...]`).

> The companion mutators `kwk::fill`, `kwk::initialize` and `kwk::destroy` are described
> with the memory algorithms; they operate on a single collection (fill a value,
> begin/end object lifetime) rather than copying across two.

@section kwk_copies_overview Functions and methods overview

| NumPy             | KIWAKU spelling                      | Semantics                        |
|-------------------|--------------------------------------|----------------------------------|
| `np.asarray(buf)` | `kwk::view(kwk::source = buf, ...)`  | borrow; **no copy**, no allocation |
| slice / tile      | `v[sl]` / `v[i,...]`                 | borrow; aliasing sub-view, **no copy**
| | value copy of a view | `auto w = v;`                     | shallow copy of the descriptor; both
alias | | value copy of an owning table | *(not allowed)*        | owning handle is move-only      |
| `a.copy()`        | `kwk::table(kwk::source = a, ...)`   | owns a fresh, independent copy  |
| `b[:] = a[:]`     | `kwk::copy(dst, src)`                | element-wise deep copy          |
| const source      | `kwk::source = cref`                 | borrowed but compile-time read-only |

The rule of thumb: **construct with `view` to borrow, construct with `table` to own, and
slice to borrow a region of either.** Deep copies happen only at `table` construction or
through the explicit `kwk::copy` algorithm — never implicitly behind an indexing
operation.

**/
