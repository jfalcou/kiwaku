#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_creation Container creation

There are four families of container creation. The first three wrap memory you already hold
(borrowing) or copy it into newly owned storage; the last one defines a space from a function
and never allocates at all.

@section kwk_creation_literals From values

NumPy lets you spell a literal array directly, e.g. `np.array([1, 2, 3])`. KIWAKU has **no
`np.array`-equivalent literal constructor today**: you cannot brace-construct a collection from a
list of values.

The supported alternative is to hold the values in a C array, a `std::array`, or a C++ range,
and wrap them — exactly the "from existing memory" forms below:

```cpp
float data[3] = {1.f, 2.f, 3.f};
auto  a      = kwk::table(kwk::source = data, kwk::shape{3_c});   // owns a copy
auto  v      = kwk::view (kwk::source = data, kwk::shape{3_c});   // borrows
```

A from-values literal is a candidate future feature (see the NOT-DONE ledger in
`kiwaku/direction-DONE.md`).

@section kwk_creation_shapes From a shape

Every container is described by a @subpage kwk_shape and (for memory-backed ones) a source.
The shape can combine runtime and compile-time extents freely:

```cpp
using namespace kwk::literals;

float src[12] = {0,1,2,3,4,5,6,7,8,9,10,11};

auto  a = kwk::table(kwk::source = src, kwk::shape{3_c, 4_c});  // static 3x4, owns a copy
auto  b = kwk::table(kwk::source = src, kwk::shape{3,  4  });   // dynamic 3x4
auto  c = kwk::table(kwk::source = src, kwk::shape{3_c, 4 });   // mixed: rows static, cols dynamic
auto  d = kwk::view (kwk::source = src, kwk::shape{12_c});      // a flat 1D view
```

When the layout of the source already determines the extents you can let the shape be derived and
only give the extents through the `kwk::of_shape` option:

```cpp
auto  e = kwk::table(kwk::source = src, kwk::of_shape = kwk::shape{2_c, 6_c});
```

A generator-backed table can also carry only a shape and no memory (see below).

@section kwk_creation_external From existing memory

The two memory-backed factories differ only in **ownership**:

- `kwk::view(kwk::source = .., kwk::shape{..})` **borrows** the source. It never allocates, never
  frees, and has the size of a pointer (plus stored dynamic extents when the shape is dynamic).
- `kwk::table(kwk::source = .., kwk::shape{..})` **owns** a fresh region and copies the source
  into it. The resulting object no longer aliases the source.

Any of these sources is accepted:

```cpp
#include <array>
#include <vector>

float           carr[4] = {1.f, 2.f, 3.f, 4.f};
std::array<float, 4> sarr = {1.f, 2.f, 3.f, 4.f};
std::vector<float>   vec  = {1.f, 2.f, 3.f, 4.f};
float const *        cptr = carr;

auto  v1 = kwk::view(kwk::source = carr, kwk::shape{4_c});  // C array
auto  v2 = kwk::view(kwk::source = sarr, kwk::shape{4_c});  // std::array
auto  v3 = kwk::view(kwk::source = vec , kwk::shape{4_c});  // contiguous range
auto  v4 = kwk::view(kwk::source = cptr, kwk::shape{4_c});  // raw pointer
```

A `const` source makes the container read-only **at compile time** — writing through it is a
compile error, not a run-time fault:

```cpp
float const cref[4] = {1.f, 2.f, 3.f, 4.f};
auto  c = kwk::view(kwk::source = cref, kwk::shape{4_c});
// c[0] = 9.f;   // does not compile: const source is read-only
```

By default the element kind is deduced from the source. To force a different scalar kind, pass
`kwk::as<T>()`:

```cpp
std::vector<double> dv = {1.0, 2.0, 3.0};
auto  i = kwk::table(kwk::source = dv, kwk::shape{3_c}, kwk::as<std::int16_t>());
```

**Struct-of-arrays (product kinds).** To build a collection whose elements have several fields —
a *product kind* — pass `kwk::as<K>()` for `K = kumi::tuple<..>` and a source made of one pointer
per field:

```cpp
#include <kumi/tuple.hpp>
using K = kumi::tuple<float, double>;

std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
std::array<double, 4> ys = {10., 20., 30., 40.};

auto  v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                   , kwk::shape{4_c}
                   , kwk::as<K>());
```

Each field becomes its own SoA *fibre*, reachable through a per-field view returned by `kwk::get<I>`
(an owning table is transposed to SoA even when the source is array-of-structures):

```cpp
auto  fx = kwk::get<0>(v);   // a view over the float  fibre, aliasing `v`
auto  fy = kwk::get<1>(v);   // a view over the double fibre
fx->kind();                  // kwk::as<float>()
```

The NumPy function `copy`/`asarray` (copy or convert an existing array) corresponds to the owning
`kwk::table(kwk::source = ..)` forms above; the whole-collection `kwk::copy(dst, src)` algorithm
covers copying between two existing containers (see @subpage kwk_copies).

@section kwk_creation_generators From a generator

A **generator-backed table** defines a space by a function and never allocates: every cell is
evaluated on demand by calling the function with its grid coordinates when read.

```cpp
constexpr auto sum = [](auto... xs) { return (xs + ...); };

auto  g = kwk::table(kwk::generator = sum, kwk::shape{3_c, 4_c});

g->ndim;       // 2
g->size();     // 12
g[0, 0] == 0;  // sum of coordinates (0,0)
g[2, 3] == 5;  // sum of coordinates (2,3)
```

Give no explicit shape and the extent comes from the generator's own option, starting as a
wildcard (extent unknown) until you provide one:

```cpp
auto  w = kwk::table(kwk::generator = sum);
w->size();     // 0 : single wildcard dimension until an explicit shape is given
```

Generators compose with interpolation and boundaries like memory-backed containers — a
`kwk::linear` generator table reads through the function at fractional positions:

```cpp
auto  t = kwk::table(kwk::generator = sum, kwk::shape{5_c}, kwk::linear);
t[2.5] == 2.5;  // linear blend of the coordinates 2 and 3
```

This is the KIWAKU counterpart of NumPy's `np.fromfunction` (define values by a function of
coordinates), with the bonus that it stays storage-less. There is no `np.arange`/`np.linspace`
value-sequence helper; generate the ramp yourself and wrap it, or write one with a constant-plus-
coordinate generator.

**/
