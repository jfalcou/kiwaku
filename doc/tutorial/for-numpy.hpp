#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_for_numpy KIWAKU for NumPy users

KIWAKU is best thought of as "NumPy that never allocates a view until you ask it to
own something". Its unifying abstract is a **collection**: an n-dimensional space over
which you read and write through a uniform, numpy-like position syntax. This page is a
side-by-side mapping from the NumPy mental model to the exact KIWAKU spelling, based
strictly on operations that the library implements and tests exercise.

@section kwk_for_numpy_vocabulary Vocabulary

| NumPy            | KIWAKU | Meaning |
|------------------|--------|---------|
| `ndarray`        | `kwk::table` / `kwk::view` / `kwk::collection` | an n-dimensional array.
`table` owns storage; `view` borrows (shallow-wraps) caller-provided memory | | `shape`          |
`kwk::shape{...}` / `->shape()` | per-axis extents, as a nested product type | | `ndim`           |
`.ndim` (compile-time `constexpr`) / `->size()` | number of dimensions / total element count | |
`strides`        | `->stride(...)` / the layout | memory offsets between consecutive elements | |
`dtype`          | `kwk::as<T>()` / `->kind()` | the element kind: scalar (`as<float>`), ordinal
(`as<unsigned>`), or product (`as<kumi::tuple<...>>`) | | `lvalue`/`rvalue` base | `kwk::source =
ptr` (const ⇒ read-only) | owned-looking vs borrowed / const vs mutable source | | array of structs
| `kwk::as<K>` product kind | a struct-of-arrays (SoA) table backed by per-field fibers | | fancy
indexing  | partial indexing (tiling) | indexing fewer axes than `ndim` returns a sub-collection
(tile) | | broadcasting    | `~` ad-hoc, coordinate options / generators | function-defined spaces
evaluated on demand (J6) | | `np.copy` / `np.fill` | `kwk::copy` / `kwk::fill` | element-wise
algorithms over the shape |

@section kwk_for_numpy_creation Creating arrays

Both tuple-like brace syntax `kwk::shape{...}` and call syntax `kwk::shape(...)` are
accepted. Extent constants (compile-time) use the ``_c`` literal suffix from
`kwk::literals`; plain integers give runtime ("dynamic") extents.

**Borrowed view over caller memory** (like `np.asarray` on a buffer):

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float        ref[4]  = {1, 2, 3, 4};
float const  cref[4] = {1, 2, 3, 4};

auto v = kwk::view(kwk::source = ref,  kwk::shape{4_c});   // mutable, borrowed
auto r = kwk::view(kwk::source = cref, kwk::shape{4_c});   // const source => read-only (r[i]=..
won't compile)
```

**Owning table** (a fresh region is allocated and the source copied in, like
`np.array`):

```cpp
auto t = kwk::table(kwk::source = ref, kwk::shape{4_c});
```

**Without a source** the shape alone gives an uninitialised owner; combine with
`kwk::fill` or a generator. A 2D `table` is just a nested shape:

```cpp
auto m = kwk::table(kwk::source = src, kwk::shape{2_c, 3_c});   // 2 x 3 matrix, row-major
```

**Function-defined (no memory at all)** — the closest thing to a lazy/broadcast
expression, evaluated on demand from the grid coordinates:

```cpp
constexpr auto sum_of = [](auto... xs) { return (xs + ...); };
auto t = kwk::table(kwk::generator = sum_of, kwk::shape{3_c, 4_c});  // t[i,j] == i + j
```

@section kwk_for_numpy_indexing Indexing and slicing

**Scalar indexing** is `t[i]` for 1D and `t[i, j]` for 2D — the comma operator spells a
multi-dimensional position:

```cpp
v[0]        // first element (like v[0] / v[0,0])
m[1, 2]     // row 1, column 2
```

Partial (fewer indices than `ndim`) indexing yields a **tiled sub-collection** rather
than a scalar — exactly NumPy's "indexing with fewer indices returns an array":

```cpp
auto v = kwk::view(kwk::source = ref, kwk::shape{3, kwk::shape{2, 2}}); // 3 tiles of 2x2
v[1]           // the second 2x2 tile (a collection, not a number)
v[1][0, 1]     // then an element inside it
```

**Slicing** mirrors `a[start:stop:step]` via `kwk::slicer` (and the helper
`kwk::to_slicer`, `kwk::end`):

```cpp
kwk::slicer{2, 6, 2};   // a[2:6:2]
kwk::end / 2;           // a[:len/2]
v[kwk::slicer{2, 4, 1}] // a[2:4]
```

A slice returns a **borrowed sub-view** that aliases the parent: writing through it is
visible in the parent (like NumPy views).

@section kwk_for_numpy_boundary Boundary conditions and interpolation

Where NumPy has `np.take(..., mode=...)`, KIWAKU lets you read **outside** the domain
by attaching a boundary condition and, for non-integer coordinates, an interpolator:

```cpp
using kwk::_;
auto v_c = kwk::view(kwk::source = ref, kwk::shape{N}, kwk::clamp);  // out-of-range clamps
(a[-1]==a[0]) auto v_t = kwk::view(kwk::source = ref, kwk::shape{N}, kwk::tile);   // periodic wrap
(np.tile semantics) auto v_m = kwk::view(kwk::source = ref, kwk::shape{N}, kwk::mirror); // reflect
/ symmetric auto v_l = kwk::view(kwk::source = ref, kwk::shape{5_c}, kwk::linear); // +
interpolation at f.p. coords t[2.5]       // linear blend of t[2] and t[3]  (with the linear
interpolator engaged)
```

Interpolators include `kwk::linear`, `kwk::nearest`, `kwk::floor`, `kwk::ceil`,
`kwk::round`; boundary conditions `kwk::clamp`, `kwk::tile`, `kwk::mirror`.

@section kwk_for_numpy_soa Structured arrays (dtype with fields / array-of-structs)

A **product kind** is the KIWAKU analogue of a NumPy structured array. It is declared
with `kwk::as<kumi::tuple<T...>>()` and reads/writes field fibres through
`kwk::get<I>`, which yields a per-field scalar collection that **aliases** the parent:

```cpp
using K = kumi::tuple<float, double>;

std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
std::array<double, 4> ys = {10., 20., 30., 40.};

auto v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                  , kwk::shape{4_c}
                  , kwk::as<K>());        // SoA: two parallel fibres

kwk::get<0>(v)[1] = 111.f;                // write through field 0
auto fx = kwk::get<0>(v);                 // a scalar (float) 1D collection
```

A range of records (`std::vector<kumi::tuple<...>>`, an array-of-structs) is
transposed to SoA on ownership.

@section kwk_for_numpy_algorithms Whole-array operations

| NumPy | KIWAKU |
|-------|--------|
| `b[:] = a[:]` | `kwk::copy(out, in)` |
| `a[:] = v`    | `kwk::fill(a, v)` |
| `np.fromfunction` / broadcast | `kwk::table(kwk::generator = f, kwk::shape{...})` |
| struct field access | `kwk::get<I>(a)` |

Example:

```cpp
kwk::fill(t, 0.0f);            // like np.zeros in-place / a[:] = 0
kwk::copy(out, in);            // element-wise out[i..] = in[i..]
```

@section kwk_for_numpy_pitfalls Pitfalls

- **`view` borrows, `table` owns.** A `view` never allocates and never outlives its
  source; a `table` copies into freshly owned memory (or is generator-backed and
  carries no storage at all). Choose by ownership intent, not by API mood.
- **Const source ⇒ read-only at compile time.** Wrapping a `const` array with
  `kwk::source = cref` makes writes a compile error, not a runtime fault.
- **Slices and tiles alias.** Modifying a sub-view modifies the parent. This is
  NumPy-view semantics roped into the type system, so it can't silently surprise
  you at runtime.
- **A generator `table` re-evaluates every read.** It never allocates (`sizeof` can be
  1) but is not cached: `t[i]` calls the function each time. Treat it as a pure
  expression, not storage.
- **Product-kind fields are parallel fibres.** `kwk::get<I>(a)` returns a collection
  over one field; the record tuple itself is not a contiguous cell to index in the
  SoA case. Setting `kwk::as<K>` from an array-of-structs transposes to SoA on
  ownership.
- **Partial indexing yields a collection, not a number.** `v[1]` on a 3×2×2 space is a
  2×2 sub-collection; scalar-read only with the full coordinate list. This is the
  main place numpy-trained intuition can slip.

**/
