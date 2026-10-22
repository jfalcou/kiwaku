#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_shape Shape manipulation

NumPy offers a rich set of shape-changing operations (`reshape`, `ravel`, `transpose`,
`stack`, `concatenate`, `split`, broadcasting). KIWAKU's shape model is different and
deliberately smaller: a collection's **extents are fixed at construction** by
`kwk::shape{...}` / `kwk::of_shape`, and there is **no in-place mutating reshape**.
Changing how you *view* the same memory — a new shape, a different storage order, or a
split into sub-ranges — is done by constructing a fresh borrowed `view` over the same
buffer, or by slicing. This page maps each common NumPy shape operation onto what KIWAKU
actually provides, and calls out what it deliberately does not.

@section kwk_shape_changing Changing the shape of a container

A shape is a nested tuple of extents; each extent is either a compile-time constant
(``_c`` literal) or a runtime integer:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

auto a = kwk::view(kwk::source = buf, kwk::shape{4_c});            // 1-D, static
auto m = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c});       // 2-D, static
auto d = kwk::view(kwk::source = buf, kwk::shape{n});              // 1-D, runtime extent n
```

**There is no `reshape`/`ravel` member.** A KIWAKU collection does not re-interpret its
own extents in place — the shape is part of its type and its storage intent, fixed at
construction. To read the same contiguous buffer under a different shape, construct a
second borrowed `view` over the same source:

```cpp
float buf[6] = {0, 1, 2, 3, 4, 5};

auto m = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c});   // 2 rows x 3 columns
auto r = kwk::view(kwk::source = buf, kwk::shape{6_c});        // same bytes as a flat 1-D view
```

Both `m` and `r` borrow `buf` and alias the same memory, so this is the analogue of
`np.reshape` *as a view* (it never copies). `kwk::of_shape` lets the shape be derived
from the source whenever the container is built from memory that already encodes its own
extents.

Shapes themselves stay composable at the value level: `->shape()` returns the extents,
`->size()` the total element count, and `ndim` the (compile-time) number of dimensions:

```cpp
auto s = m->shape();   // kwk::shape{2, 3}  (2_c x 3_c)
auto k = m->size();    // 6
auto n = m.ndim;       // 2  (constexpr)
```

**Storage order (the transpose/swapaxes analogue).** The physical order in which the
logical dimensions are laid out in memory is chosen at construction with a storage-order
option — `row_major_order` (default) or `column_major_order`:

```cpp
auto c = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c}, kwk::column_major_order);
```

This is closer to controlling `strides`/`order` at allocation than to a `transpose()`
call: the *logical* index space is unchanged, only the physical mapping is chosen up
front. There is **no `transpose()`/`swapaxes()`/`permute()` member** that rearranges the
axes of an existing collection; to present the same memory under a transposed logical
order, build a view over the same buffer with the complementary storage order (or, more
simply, index the original with swapped coordinates — `a[j, i]` is just a nested/2-D
read).

@section kwk_shape_stacking Stacking together different containers

**KIWAKU has no `stack`/`concatenate`/`hstack`/`vstack` and no public "join"
operation.** A collection's storage is either borrowed (a `view` over one contiguous
source) or owned (a `table` over one allocated region); there is no facility that
allocates a new region and copies several input collections side by side in a single
call.

The supported equivalent is explicit: create an owning destination `table` with the
combined shape, then `kwk::copy` each source into its own slice:

```cpp
#include <kiwaku/kiwaku.hpp>
float  xs[2] = {1, 2};
float  ys[3] = {3, 4, 5};

auto a = kwk::view(kwk::source = xs, kwk::shape{2_c});
auto b = kwk::view(kwk::source = ys, kwk::shape{3_c});

auto dst = kwk::table(kwk::source = dst_backing, kwk::shape{5_c});   // owning, length 5
kwk::copy(dst[kwk::slicer{0, 2, 1}], a);   // dst[0..2) = a
kwk::copy(dst[kwk::slicer{2, 5, 1}], b);   // dst[2..5) = b
```

The slicing step picks the destination sub-range and the `kwk::copy` algorithm does the
element-wise assignment (`dst[i..] = src[i..]`).

@section kwk_shape_splitting Splitting one container into several smaller ones

Splitting is the one shape family that is **first-class**: it is exactly slicing. Where
NumPy has `split`/`hsplit`/`vsplit`, KIWAKU cuts a contiguous buffer with
`kwk::slicer` (or `kwk::to_slicer` / the `_` wildcard) and gets independent **borrowed
sub-views** that alias the parent:

```cpp
using kwk::_;
float buf[6] = {0, 1, 2, 3, 4, 5};
auto  v     = kwk::view(kwk::source = buf, kwk::shape{6_c});

auto first_half = v[kwk::slicer{0, 3, 1}];   // elements 0,1,2
auto second_half= v[kwk::slicer{3, 6, 1}];   // elements 3,4,5
auto every_two  = v[kwk::slicer{0, 6, 2}];   // elements 0,2,4
```

Every slice is a *view*, not a copy: writing through a slice is visible in the parent and
through the other slices (see @subpage kwk_slicing). Splitting a multi-dimensional
container is the same operation with one slicer per kept dimension and `_` for the dims
you keep whole.

@section kwk_shape_broadcasting Broadcasting

**Broadcasting is not implemented.** NumPy aligns trailing dimensions automatically so
that `a + b` over mismatched shapes "just works"; KIWAKU has no element-wise operator
layer (no ufuncs — see @subpage kwk_universal) and no automatic trailing-dimension
alignment. Two collections only read/write each other when their shapes agree for the
operation performed (positional indexing or `kwk::copy`).

The supported patterns are explicit rather than automatic:
- **Repeat a vector/row** by indexing a `view` with `_` over the broadcast axis, or by
  making the shared dimension a tile (a nested shape) and indexing the tile.
- **Function-defined spaces** (a generator table, `kwk::generator =`) produce a value on
  demand at every grid coordinate — the closest thing to a lazy broadcast expression.

This is a deliberate scope decision: broadcasting is rowed into the not-done ledger
(`kiwaku/direction-DONE.md`) because it only earns its keep as part of an element-wise
expression layer that KIWAKU does not yet have.

**/