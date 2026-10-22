#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_interoperability Interoperability with other libraries

KIWAKU is header-only and has no runtime dependency beyond its companion library
(Kumi). Interoperability is therefore the opposite of a binding layer: it is the ability
to **wrap someone else's memory as a collection** with `kwk::view`/`kwk::table` and a
`kwk::source` — no conversion, no intermediate copy, and no foreign-object protocol.
Anything already contiguous in memory (the standard library's containers, C arrays, raw
buffers, records from another library) becomes a KIWAKU collection over that same memory.

@section kwk_interoperability_std The standard library

Every contiguous standard-library container is a valid `kwk::source`: a `view` borrows
its storage (no copy, no allocation) and a `table` owns a fresh copy. This is the NumPy
`np.asarray`-over-a-buffer analogue.

```cpp
#include <kiwaku/kiwaku.hpp>
#include <vector>
#include <array>
using namespace kwk::literals;

std::vector<float> vec = {1.f, 2.f, 3.f, 4.f};
auto v = kwk::view(kwk::source = vec, kwk::shape{4_c});   // borrows vec's storage

std::array<double, 4> arr = {10., 20., 30., 40.};
auto a = kwk::view(kwk::source = arr, kwk::shape{4_c});   // borrows arr's storage

float c_arr[4] = {0, 1, 2, 3};
auto c = kwk::view(kwk::source = c_arr, kwk::shape{4_c}); // borrows the C array
```

Writes through a `view` land in the source container: `v[0] = 9.f` modifies `vec[0]`,
so KIWAKU can operate on data owned by the standard library in place. A `const` source
(`std::array<float,N> const`, a `const` C array) makes the view **read-only at compile
time**.

Element-wise / whole-array operations on such a view use the public algorithms directly:
`kwk::fill(v, 0.0f)`, `kwk::copy(dst, v)`.

@section kwk_interoperability_simd SIMD types

The element **kind is a C++ type** (`kwk::as<T>()` / `->kind()`), so a SIMD register type
is a perfectly valid element kind: a collection of SIMD vectors is just a collection
whose `value_type` is that register type. No special dtype is required and no conversion
occurs — the stored cells are the register values themselves.

```cpp
using vec_t = /* a SIMD / fixed-width register type */;
auto simd = kwk::view(kwk::source = regs, kwk::shape{N}, kwk::as<vec_t>());
```

Because the geometry (shape/stride/accessor) is decoupled from the memory scheme, a
compound kind also works: `kwk::as<kumi::tuple<T...>>()` lays out records
struct-of-arrays (see @subpage kwk_data_types), which pairs naturally with
per-component SIMD access through @subpage kwk_indexing.

@section kwk_interoperability_external Foreign container types

The `kwk::source` contract is intentionally minimal — a pointer (or tuple of pointers)
plus an extent — so **any foreign contiguous buffer** can be wrapped without the foreign
type being KIWAKU-aware. The supported source forms are a raw pointer, a C array, a
contiguous range, and a tuple of per-field pointers:

```cpp
// Wrap a raw, caller-owned buffer as a borrowed 2x3 collection:
float* foreign  = /* ... */;
auto   grid     = kwk::view(kwk::source = foreign, kwk::shape{2_c, 3_c});

// Wrap a non-uniform stride explicitly (e.g. an interleaved/AoS buffer) with of_stride:
auto strided = kwk::view(kwk::source = foreign
                       , kwk::shape{2_c, 3_c}
                       , kwk::of_stride = kwk::stride{1_c, 2_c});
```

**Array-of-structures records from another library** map onto a product kind. Given a
foreign record type `Record` (any struct), the record is transported as a `kumi::tuple`
of its fields; a range of records (`std::vector<Record>`-like AoS) is transposed to
struct-of-arrays when the owning `table` copies it in, and a tuple of per-field pointers
is wrapped directly:

```cpp
using K = kumi::tuple<float, double>;

std::vector<kumi::tuple<float, double>> points = /* foreign AoS records */;
auto soa = kwk::table(kwk::source = points, kwk::as<K>());      // transposed to SoA

// or wrap already-separated per-field arrays (no copy):
std::array<float,4>  xs;  std::array<double,4>  ys;
auto v = kwk::view(kwk::source = kumi::tuple{xs.data(), ys.data()}, kwk::shape{4_c}, kwk::as<K>());
```

This is the buffer-protocol story without a protocol: as long as your data is contiguous
(interleaved or strided, records or parallel arrays), KIWAKU addresses it in place
through @subpage kwk_indexing and the memory algorithms.

**/