#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_universal Universal functions

NumPy's *ufuncs* apply a function element-wise over an array (`np.add`, `np.sin`,
`np.multiply` ...), with reductions (`np.sum`, `np.mean`, `np.prod`) built on the same
engine. **KIWAKU has no universal-function layer and no reductions.** There is no
element-wise operator algebra, no `sum`/`mean`/`product`, and no public "apply a functor
to every element" mechanism beyond the explicit whole-collection algorithms
(`kwk::copy`, `kwk::fill`, `kwk::initialize`, `kwk::destroy`). This page documents what
does exist and gives the idiomatic manual pattern for what does not.

@section kwk_universal_elementwise Element-wise application

KIWAKU provides **four whole-collection algorithms**, each iterating the collection's
n-dimensional shape and acting on the element at every position:

| Name | Invocation | Effect |
|------|-----------|--------|
| `kwk::copy`   | `kwk::copy(out, in)`        | `out[i..] = in[i..]` for every position |
| `kwk::fill`   | `kwk::fill(c, value)`       | `c[i..] = value` for every position |
| `kwk::initialize` | `kwk::initialize(c)`    | start the object lifetime of every element |
| `kwk::destroy` | `kwk::destroy(c)`          | end the object lifetime of every element |

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

float data[6] = {1, 2, 3, 4, 5, 6};
auto  v      = kwk::view(kwk::source = data, kwk::shape{2_c, 3_c});

kwk::fill(v, 0.0f);              // like v[:] = 0  (in place)

float out[6] = {};
auto  d      = kwk::view(kwk::source = out, kwk::shape{2_c, 3_c});
kwk::copy(d, v);                 // like np.copyto(d, v): d[i..] = v[i..]
```

**There is no `np.add`/`np.sin` operator.** `v + w`, `v * 2`, `kwk::sin(v)` do not
exist: element-wise arithmetic is not fused into an expression layer. (This is a
deliberate scope row — see `kiwaku/direction-DONE.md` — because such an algebra is a
large subsystem and KIWAKU instead keeps element access cheap and explicit via
@subpage kwk_indexing.)

The supported "element-wise apply" is an explicit position loop, which can be written
uniformly over any rank by folding the shape:

```cpp
#include <cmath>
for (std::size_t i = 0; i < v->size(); ++i)
  v[i] = std::sin(v[i]);                 // element-wise transform, spelled by hand
```

For a multi-dimensional space, nest over the axes (or iterate `->indexes(dim)` along each
dimension) — the same Cartesian walk the algorithms above perform internally.

@section kwk_universal_reductions Reductions

**KIWAKU has no reduction primitives** — no `sum`, `mean`, `prod`, `min`/`max` over a
collection, and no `axis=`-style partial reduction. (Its `extremum` utilities are
single-scalar traits, not array aggregates.)

The supported pattern is an explicit accumulator fold over the elements:

```cpp
double acc = 0.0;
for (std::size_t i = 0; i < v->size(); ++i) acc += v[i];
double mean = acc / v->size();           // reduce + normalize, by hand
```

Because element access is a plain `operator[]` on the collection (with boundary /
interpolation available, see @subpage kwk_indexing), writing a custom reduction is
straightforward and keeps the aggregation logic visible rather than hidden behind a named
reduction engine.

@section kwk_universal_custom Bringing your own

NumPy lets you register your own ufunc; KIWAKU has no such registry. What it does offer is
a seam for **your own whole-collection algorithm**: the internal Cartesian walk over a
shape (`kwk::__::for_each`, used to implement `copy`) is the same loop you can write in
your own namespace to apply any functor element-wise:

```cpp
namespace my
{
  template<kwk::concepts::collection C, typename F>
  constexpr void for_each(C& c, F f)
  {
    for (std::size_t i = 0; i < c->size(); ++i)
      f(c[i]);
  }
}

my::for_each(v, [](float& x) { x = x * x + 1; });
```

There is no public "ufunc protocol" — the element kind is a C++ type, not a dispatch tag,
so an element-wise op on a product (SoA) collection reads/writes each field through
`kwk::get<I>` rather than through a single element callback. For fields, apply your
functor per fibre:

```cpp
my::for_each(kwk::get<0>(rec), [](float& x){ x *= 2; });
```

The takeaway: **element-wise maths and reductions are done with explicit loops and
accumulators**, not with a universal-function layer; `kwk::copy`/`kwk::fill`/
`kwk::initialize`/`kwk::destroy` are the only built-in whole-collection algorithms.

**/