#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_data_types Data types

Every element of a collection has a **kind** — its C++ type. Where NumPy has a fixed
`dtype` vocabulary (`np.float64`, `np.int32`, ...) chosen per-array, KIWAKU's kind is
simply the concrete element type, stated with `kwk::as<T>()` at construction and
queryable through `->kind()`. A collection over any C++ type is valid, including
compound/struct and user-defined types, and a *product kind* (`kwk::as<kumi::tuple<
...>>`) gives the analogue of a NumPy structured array.

@section kwk_data_types_supported Supported element types

Every C++ type can be a kind, but the common scalars have ready-made constants that read
like NumPy's dtype names:

| NumPy dtype | KIWAKU kind constant | `kwk::as<T>()` spelling |
|-------------|----------------------|--------------------------|
| `np.float32` | `kwk::real32` | `kwk::as<float>()` |
| `np.float64` | `kwk::real64` | `kwk::as<double>()` |
| `np.int8/16/32/64` | `kwk::int8` … `kwk::int64` | `kwk::as<std::int8_t>()` … |
| `np.uint8/16/32/64` | `kwk::uint8` … `kwk::uint64` | `kwk::as<std::uint8_t>()` … |

The kind is set as an option on a `view` or `table`; if none is given it is deduced from
the source (or defaults to `kwk::real64`):

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float          buf[4]   = {0, 1, 2, 3};
auto           v        = kwk::view(kwk::source = buf, kwk::shape{4_c});   // kind deduced: float
auto           ti       = kwk::table(kwk::source = buf, kwk::shape{4_c}, kwk::as<std::int16_t>());
```

**Introspection.** `->kind()` returns the kind option; `->kind()->element_type` (or the
collection's `value_type`) names the C++ type, and `->kind()->itemsize` its size in
bytes:

```cpp
auto k  = v->kind();                   // the kind option (kwk::as<float>())
using T = typename decltype(v)::value_type;   // float
auto sz = v->kind()->itemsize;                // sizeof(float)
```

Because the kind is a *type* rather than a tag enum, **any C++ type works** — the element
type is not restricted to arithmetic types. A `std::array`/struct of values, a
fixed-width numeric type, or a user-defined record can all be the element kind of a
collection, which is what the product and compound rows below build on.

**Compound / SIMD kinds.** The kind machinery is type-agnostic, so a SIMD register type
or a compound value type can be the element kind too. Interoperating with such types
through custom sources/accessors is covered on @subpage kwk_interoperability; the kind
itself places no limitation.

**Structured arrays (product kinds).** A NumPy *structured array* — a record with typed
fields — maps onto a **product kind**: `kwk::as<kumi::tuple<T0, T1, ...>>()`. Such a
collection is stored *struct-of-arrays*: each field occupies its own contiguous fibre in
the shared region, and per-field views are obtained with `kwk::get<I>` (which aliases the
parent, so writes flow back):

```cpp
using K = kumi::tuple<float, double>;

std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
std::array<double, 4> ys = {10., 20., 30., 40.};

auto rec = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{4_c}
                    , kwk::as<K>());        // SoA: a float fibre + a double fibre

auto fx = kwk::get<0>(rec);                 // a scalar (float) collection over field 0
auto fy = kwk::get<1>(rec);                 // a scalar (double) collection over field 1
kwk::get<0>(rec)[1] = 111.f;                // write through field 0, visible in the parent
```

A range of records (an array-of-structures such as `std::vector<kumi::tuple<...>>`) is
transposed to SoA on the owning `table` automatically. See @subpage kwk_creation for the
source forms and the SoA construction details.

@section kwk_data_types_conversions Conversions

**There is no `astype`.** NumPy can convert an array's dtype in place with `a.astype(
np.float32)`; KIWAKU has no kind-mutating member, and a collection's kind (like its
shape) is fixed at construction because the storage layout — cell size, and for a
product kind the per-field fibres — depends on it.

To change the element kind, build a destination collection with the target kind and copy
element-wise, applying the conversion explicitly:

```cpp
double         in[4] = {1.0, 2.0, 3.0, 4.0};
auto           a     = kwk::view(kwk::source = in, kwk::shape{4_c});   // kind double

auto           b     = kwk::table(kwk::source = bytes, kwk::shape{4_c}, kwk::real32);
// element-wise narrowing conversion (double -> float), only where you opt in:
for (std::size_t i = 0; i < 4; ++i) b[i] = static_cast<float>(a[i]);
```

The `static_cast` makes the precision change explicit — there is no implicit silent
dtype conversion, which is exactly what the `astype` spelling hides today. For a *record*
conversion (turning a uniform kind into a product kind, or vice versa), construct the
target `table` with `kwk::as<kumi::tuple<...>>()` and fill each field from the source.

**/