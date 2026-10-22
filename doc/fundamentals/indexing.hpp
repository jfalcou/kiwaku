#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_indexing Indexing on containers

@section kwk_indexing_basics Indexing, slicing and iterating

A positional index reads (or writes) the cell at a position. Use **one index per dimension** to
get a scalar; the outer dimension comes first, exactly like NumPy:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float   grid[12] = {0,1,2,3,4,5,6,7,8,9,10,11};
auto    a        = kwk::view(kwk::source = grid, kwk::shape{3_c, 4_c});

a[0, 0];      // reads  grid[0]  == 0
a[2, 3];      // reads  grid[11] == 11
a[1, 2] = 9;  // writes grid[6]
```

Indexing **writes through**: because a `view` borrows its source, assigning to a cell updates the
underlying memory (visible in the source and in any other view over it).

**Partial indexing and tiles.** When the shape is *nested* (a grid of sub-shapes — a *tile
layout*), indexing with fewer indices than the full dimensionality returns a **tiled
sub-collection** that aliases the parent, rather than a scalar:

```cpp
auto v = kwk::view(kwk::source = grid, kwk::shape{3_c, kwk::shape{2_c, 2_c}});  // 3 tiles of 2x2

auto tile = v[1];              // the inner 2x2 block of tile 1, aliasing `grid`
tile[0, 1];                    // reads inside that block
```

With a 2-dimensional grid of tiles, partial indexing takes several indices at once:

```cpp
auto u = kwk::view(kwk::source = grid, kwk::shape{kwk::shape{2_c, 2_c}, kwk::shape{2_c, 2_c}});
auto t = u[1, 0];              // the inner block at grid row 1, column 0, aliasing `grid`
```

**Out-of-domain and fractional reads.** Which positions are legal — and what a read returns — is
decided by the container's *accessor*. With the default boundary (`clamp`), reading past the end
clamps to the last cell; `tile` makes the domain periodic; `mirror` reflects it:

```cpp
auto c = kwk::view(kwk::source = grid, kwk::shape{12_c}, kwk::clamp);
auto p = kwk::view(kwk::source = grid, kwk::shape{12_c}, kwk::tile);
auto m = kwk::view(kwk::source = grid, kwk::shape{12_c}, kwk::mirror);

c[-1] == c[0];      // clamped to the first cell
p[12] == p[0];      // periodic
m[12] == m[11];     // mirrored back
```

With the `linear` interpolator, fractional positions blend the surrounding cells (see
@subpage kwk_creation for generator tables doing the same thing without memory).

**Iterating** a container means reading every cell positionally — KIWAKU's whole-collection
algorithms (`kwk::copy`, `kwk::fill`, …) do this for you; see @subpage kwk_universal.

Slicing (ranges along dimensions, `a[start:stop:step]`) is covered on its own page,
@subpage kwk_slicing. Indexing with `kwk::get<I>` on a product kind (field access) is covered in
@subpage kwk_data_types.

@section kwk_indexing_arrays Indexing with containers of indices

NumPy *fancy indexing* selects cells using an array of indices, e.g.
`a[[0, 2, 5]]`. KIWAKU does **not** support indexing with a container of indices today. There is
no `operator[]` overload that takes a collection of positions.

To gather a subset of a container, build a destination container and copy the wanted cells into
it with `kwk::copy` (or write the loop yourself — reading is just `a[i]`).

@section kwk_indexing_boolean Indexing with boolean containers

NumPy *boolean-mask indexing* (`a[mask]`, `a[a > 0]`) selects every cell where a boolean
condition holds. KIWAKU has **no** boolean-mask indexing. There is no faithful C++ equivalent
without a run-time indirection layer, so it is deliberately not provided; select cells with an
explicit index loop over the condition.

@section kwk_indexing_mesh Building index meshes

NumPy's `np.indices` / `np.mgrid` build whole grids of index tuples. KIWAKU does **not** provide
an index-mesh builder. If you need a position grid, generate it as a regular container (a
coordinate-valued generator table covers most uses without storing anything; see
@subpage kwk_creation).

@section kwk_indexing_strings Indexing with strings

NumPy lets you select structured-array fields by name (`a["x"]`). String-indexed field access is
**not** supported — it does not make sense for a statically-typed C++ kind. For a product kind
(struct-of-arrays), address fields by position with `kwk::get<I>`:

```cpp
using K = kumi::tuple<float, double>;

float       fs[4] = {1.f, 2.f, 3.f, 4.f};
double      ds[4] = {10., 20., 30., 40.};

auto s = kwk::table(kwk::source = kumi::tuple{fs, ds}, kwk::shape{4_c}, kwk::as<K>());

auto  x = kwk::get<0>(s);   // the float  field, aliasing `s`
auto  y = kwk::get<1>(s);   // the double field
```

The field ordering is fixed by the kind at compile time, which is what replaces the name lookup.
See @subpage kwk_data_types.

**/
