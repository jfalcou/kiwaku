#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_for_matlab KIWAKU for MATLAB users

If you come from MATLAB, every operation you expect is either a small spelling change
(`()` → `[]`) or an explicit ownership choice (borrow vs own). This page maps the MATLAB
mental model onto the exact KIWAKU spelling, grounded strictly in operations the library
implements and tests exercise. The companion page @subpage kwk_for_numpy covers the same
ground from the NumPy side.

@section kwk_for_matlab_vocabulary Vocabulary

| MATLAB       | KIWAKU | Meaning |
|--------------|--------|---------|
| matrix / array | `kwk::table` / `kwk::view` / `kwk::collection` | an n-dimensional array. `table` owns storage; `view` borrows caller memory |
| `size(A)`    | `->shape()` / `->size()` / `.ndim` | per-axis extents / element count / number of dims |
| `class(A)` / numeric type | `kwk::as<T>()` / `->kind()` | element type, stated with `as<T>`; see @subpage kwk_data_types |
| `A(i,j)`     | `A[i, j]` | scalar positional read/write (comma indexing) |
| `A(:,1:2)`   | `A[_ , kwk::slicer{0,2,1}]` | slicing a range along an axis (0-based) |
| `A(1)` vs `A(i,j)` | `A[i]` vs `A[i, j]` | 1-D scalar read equally by position |
| struct array | `kwk::as<kumi::tuple<...>>` product kind | a struct-of-arrays (SoA) table, fields via `kwk::get<I>` |
| `reshape(A, m, n)` | a second `view` over the same buffer | re-view the same memory under new extents |
| `end`         | `kwk::end` | the last index sentinel in a slicer |

@section kwk_for_matlab_equivalences Rough equivalences

**Indexing and base index.** MATLAB is 1-based; KIWAKU is 0-based by default
(`kwk::c_index`), but can be 1-based with `kwk::fortran_index`, which shifts the index
origin so index `1` addresses element 0:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float data[4] = {10, 20, 30, 40};

auto v = kwk::view(kwk::source = data, kwk::shape{4_c});          // 0-based: v[0]==10
auto w = kwk::view(kwk::source = data, kwk::shape{4_c}, kwk::fortran_index);  // 1-based: w[1]==10
```

Comma indexing mirrors MATLAB's `A(i,j)`:

```cpp
auto m = kwk::view(kwk::source = data9, kwk::shape{3_c, 3_c});    // 3x3
float x = m[1, 2];        // element, row 1 column 2  (0-based by default)
```

**Slicing.** MATLAB `A(i:j)` / `A(i:j:k)` become `kwk::slicer` (start inclusive, stop
exclusive, step) with the `_` wildcard for "everything":

```cpp
using kwk::_;
auto rows = m[_ , kwk::slicer{0, 2, 1}];       // all rows, columns 0..1   (a 3x2 sub-view)
auto tail = v[kwk::slicer{2, kwk::end, 1}];    // from index 2 to the end
auto step = v[kwk::slicer{0, 4, 2}];           // every other element
```

MATLAB `end` is `kwk::end`; `A(:, 1:end-1)` maps to
`A[_ , kwk::slicer{0, kwk::end - 1, 1}]` (remember the stop is exclusive and 0-based).
Slices are borrowed views that alias the parent — writing through them edits the parent,
like MATLAB's true sharing.

**Column-major layout.** MATLAB stores column-major; request the same storage order with
`kwk::column_major_order`:

```cpp
auto c = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c}, kwk::column_major_order);
```

**Struct arrays (records).** A MATLAB struct array maps onto a product kind; fields are
read/written as per-field fibres via `kwk::get<I>` (which aliases the parent), and an
array-of-structs source is transposed to SoA on ownership:

```cpp
using K = kumi::tuple<float, double>;
std::vector<kumi::tuple<float, double>> pts;          // array of records
auto soa = kwk::table(kwk::source = pts, kwk::as<K>()); // transposed to parallel fields
kwk::get<0>(soa)[2] = 1.5f;                            // write field 0 of record 2
```

**reshape.** MATLAB `reshape` reinterprets the same data under new extents. KIWAKU has no
mutating `reshape`; read the same buffer under different extents by building a second
`view` (both borrow and alias the same bytes — see @subpage kwk_shape for the full story,
including the explicit-copy alternative).

**Whole-array ops.** Where MATLAB has `A(:) = v` and `B(:) = A(:)`, KIWAKU provides
`kwk::fill(A, v)` and `kwk::copy(B, A)`; function-defined grids (`ndgrid`-style) map to a
generator table `kwk::table(kwk::generator = f, kwk::shape{...})`.

@section kwk_for_matlab_pitfalls Pitfalls

- **Mind the base index.** Digits are never 1-based unless you opt in with
  `kwk::fortran_index`; a slice's stop is *exclusive* too. Porting a MATLAB slice
  verbatim tends to be off by one.
- **`view` borrows; `table` owns.** MATLAB functions can share or copy nearly freely; in
  KIWAKU, borrow with `view`, own with `table`, and remember sub-views alias.
- **Slices and tiles alias.** `A[_ , sl]` is a view into `A`; modifying it modifies `A`.
  This is the useful MATLAB-sharing behaviour, but it means you don't get a fresh array
  for free.
- **Partial indexing returns a collection, not a scalar.** `A[1]` on a matrix is a
  row/tile sub-collection, not a number (scalar-read only with the full coordinate list).
- **No element-wise operator algebra.** There is no `A .* B` / `A + B` expression layer
  (no ufuncs or reductions — see `kiwaku/direction-DONE.md`); element-wise work is an
  explicit loop, and struct fields are accessed by position with `kwk::get<I>` rather
  than by name.

**/