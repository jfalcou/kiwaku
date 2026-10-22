#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_for_fortran KIWAKU for Fortran users

If you come from Fortran, KIWAKU shares your two most important habits — arrays are
**contiguous, column-major memory**, and the natural mental model is `a(i, j)` positional
indexing — while changing indexing from **1-based to 0-based** unless you ask for
1-based. This page maps the Fortran mental model onto the exact KIWAKU spelling, grounded
strictly in operations the library implements and tests exercise. The companion page
@subpage kwk_for_numpy covers the same ground from the NumPy side.

@section kwk_for_fortran_vocabulary Vocabulary

| Fortran          | KIWAKU | Meaning |
|------------------|--------|---------|
| array `real, dimension(..)` | `kwk::table` / `kwk::view` / `kwk::collection` | an n-dimensional array. `table` owns storage; `view` borrows caller memory |
| `size(A)` / `shape(A)` | `->size()` / `->shape()` / `.ndim` | element count / per-axis extents / number of dims |
| element type (kind) | `kwk::as<T>()` / `->kind()` | element type, stated with `as<T>`; see @subpage kwk_data_types |
| `A(i,j)` / `A(i)` | `A[i, j]` / `A[i]` | scalar positional read/write (comma indexing) |
| `A(i:j)` / `A(i:j:k)` | `kwk::slicer{..}` | slicing a range along an axis (0-based, stop exclusive) |
| `A(:,1:2)` | `A[_ , kwk::slicer{0,2,1}]` | slicing a whole axis with the `_` wildcard |
| `A(i:j)` whole, `end` | `kwk::end` | the last-index sentinel in a slicer |
| derived type / structure | `kwk::as<kumi::tuple<...>>` product kind | a struct-of-arrays (SoA) table, fields via `kwk::get<I>` |
| `reshape(A, ...)` | a second `view` over the same buffer | re-view the same memory under new extents |
| `real(kind=8)` | `kwk::real64` / `kwk::as<double>()` | element type constant |

@section kwk_for_fortran_equivalences Rough equivalences

**Indexing and base index.** Fortran is 1-based; KIWAKU is 0-based by default
(`kwk::c_index`), but can be 1-based with `kwk::fortran_index`, which shifts the index
origin so index `1` addresses element 0 — this makes `do i = 1, n`-style loops port
directly:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float data[4] = {10, 20, 30, 40};

auto v = kwk::view(kwk::source = data, kwk::shape{4_c});          // 0-based: v[0]==10
auto w = kwk::view(kwk::source = data, kwk::shape{4_c}, kwk::fortran_index);  // 1-based: w[1]==10
```

Comma indexing mirrors Fortran's `A(i,j)` — `A[i, j]` reads/writes the scalar at row `i`,
column `j` with the same coordinate spelling:

```cpp
auto m = kwk::view(kwk::source = data9, kwk::shape{3_c, 3_c});    // 3x3
float x = m[1, 2];        // element, row 1 column 2 (0-based by default)
```

**Column-major layout.** Fortran stores arrays column-major; request the same storage
order with `kwk::column_major_order` (row-major is the default, as in C):

```cpp
auto c = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c}, kwk::column_major_order);
```

**Slicing.** Fortran `A(i:j)` / `A(i:j:k)` become `kwk::slicer` (start inclusive, stop
exclusive, step). Remember digits stay 0-based unless you enabled `kwk::fortran_index`,
and the stop is exclusive:

```cpp
using kwk::_;
auto rows = m[_ , kwk::slicer{0, 2, 1}];       // all rows, columns 0..1   (a 3x2 sub-view)
auto tail = v[kwk::slicer{2, kwk::end, 1}];    // from index 2 to the end
auto step = v[kwk::slicer{0, 4, 2}];           // every other element
```

A slice returns a **borrowed view that aliases the parent** — writing through it edits the
parent, which matches Fortran's aliasing semantics for array sections.

**Derived types / structures (records).** A Fortran derived type maps onto a product
kind; fields are read/written as per-field fibres via `kwk::get<I>` (which aliases the
parent), and an array of records is transposed to struct-of-arrays on ownership:

```cpp
using K = kumi::tuple<float, double>;
std::vector<kumi::tuple<float, double>> pts;          // array of records
auto soa = kwk::table(kwk::source = pts, kwk::as<K>()); // transposed to parallel fields
kwk::get<0>(soa)[2] = 1.5f;                            // write field 0 of record 2
```

**reshape.** Fortran `reshape(A, ...)` reinterprets the same data under new extents.
KIWAKU has no mutating `reshape`; read the same buffer under different extents by building
a second `view` (both borrow and alias the same bytes — see @subpage kwk_shape for the
full story, including the explicit-copy alternative).

**Whole-array ops.** Where Fortran has `A(:) = v` and `B(:) = A(:)` / `B = A`, KIWAKU
provides `kwk::fill(A, v)` and `kwk::copy(B, A)`; function-defined grids map to a
generator table `kwk::table(kwk::generator = f, kwk::shape{...})` that evaluates on
demand.

@section kwk_for_fortran_pitfalls Pitfalls

- **Mind the base index.** You are 0-based by default; opt into 1-based with
  `kwk::fortran_index` if you prefer `do i = 1, n`. A slice's stop is *exclusive* too
  (`kwk::slicer{0, 4, 1}` is elements 0..3, not 0..4), so porting a Fortran slice
  verbatim tends to be off by one.
- **`view` borrows; `table` owns.** Fortran arrays almost always borrow the caller's
  memory; in KIWAKU borrow with `view`, own a fresh region with `table`, and remember
  sub-views alias.
- **Slices and tiles alias.** `A[_ , sl]` is a view into `A`; modifying it modifies `A`.
  This is the useful Fortran-section behaviour, but it means you don't get a fresh array
  for free.
- **Partial indexing returns a collection, not a scalar.** `A[1]` on a matrix is a
  row/tile sub-collection, not a number; scalar-read only with the full coordinate list.
- **No `where` / masked / elemental transform.** There is no element-wise operator algebra
  or `where`-style masked assignment (no ufuncs or reductions — see
  `kiwaku/direction-DONE.md`); element-wise work is an explicit loop, and struct fields
  are accessed by position with `kwk::get<I>` rather than by component name.

**/
