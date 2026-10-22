#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_performance Writing performant code

The NumPy performance advice — *vectorize, avoid Python per-element loops, avoid
copies* — maps onto three concrete, type-level properties of KIWAKU: element access is
a direct `operator[]` over memory (no interpreter in the loop), ownership is an
either/or you choose with `view` versus `table`, and sub-views are borrows that never
copy. This page turns each guidance into the KIWAKU spelling.

@section kwk_performance_access Access patterns

Element access is a plain C++ `operator[]` over the collection — there is no virtual
call and no interpreter; the accessor, boundary and interpolator are resolved at compile
time into inline code. Reading a scalar is a coordinate→flat-index→memory-read chain
whose stages (coordinate, boundary, interpolator) are all `constexpr` strategy types:

```cpp
#include <kiwaku/kiwaku.hpp>
using namespace kwk::literals;

auto v = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c});
float x = v[1, 2];        // a direct indexed read; stages are inline, not dynamic dispatch
```

For tight element-wise passes, iterate the flat space and index by the running flat
index (or fold the shape), exactly as the whole-collection algorithms (`kwk::copy`,
`kwk::fill`) do internally — so a hand-written loop gets the same code shape as the
built-ins:

```cpp
for (std::size_t i = 0; i < v->size(); ++i) v[i] = std::sqrt(v[i]);
```

Choosing **compile-time extents** (the ``_c`` literal) instead of runtime integers lets
the stride arithmetic and the storage footprint be fully static, which lets the compiler
unroll and keep the data in registers:

```cpp
auto fast = kwk::view(kwk::source = buf, kwk::shape{4_c});   // static: strides known at compile time
auto dyn  = kwk::view(kwk::source = buf, kwk::shape{n});     // runtime: address arithmetic per read
```

For out-of-domain or fractional reads, the boundary (`clamp`/`tile`/`mirror`) and
interpolator (`linear`/`nearest`/`floor`/`ceil`/`round`) strategies are selected at type
time too — see @subpage kwk_indexing — so enabling them costs little more than an
unselected branch, and an accessor that needs none of them carries no state.

@section kwk_performance_copies Avoiding needless copies

**Borrow by default.** A `view` shallow-wraps caller memory: it allocates nothing, and
its footprint is a pointer plus a shape descriptor (a fully-static 1-D view is a single
pointer). Prefer `view` whenever you do not need to outlive the source.

```cpp
auto v = kwk::view(kwk::source = buf, kwk::shape{4_c});   // no allocation, ^= sizeof(void*)
```

**Own only where you must.** A `table` copies its source into a fresh region; a **static**
shape stores it *inline* in the object (no heap), while a runtime shape owns a single
heap blob:

```cpp
auto s = kwk::table(kwk::source = buf, kwk::shape{4_c});  // inline stack storage, no heap
auto h = kwk::table(kwk::source = buf, kwk::shape{n});    // one heap allocation
```

**Sub-views never copy.** Slicing (`kwk::slicer`) and tile indexing produce borrowed
collections that re-point to the parent's bytes — there is no way to accidentally get a
copy from an indexing operation. Write-through aliases the parent; if you do want an
independent copy you must ask for it explicitly (`kwk::table(kwk::source = ...)` or
`kwk::copy(dst, src)`), which keeps copies visible in the source (see
@subpage kwk_copies).

**Function-defined spaces allocate nothing.** A `kwk::generator` table stores a function
(no memory at all — `sizeof` can be 1) and evaluates each read on demand:

```cpp
auto t = kwk::table(kwk::generator = [](auto... c){ return (c + ...); }, kwk::shape{3_c, 4_c});
```

Use it for on-demand / implicit grids when materializing storage would be the
bottleneck; remember it re-evaluates per read (it is an expression, not a cache).

`kwk::copy` between borrowed views writes element-wise in place, so a "copy" of a
foreign buffer into an owned `table` is a single contiguous pass, and `kwk::fill` is a
single per-element store — both implemented as tight loops over the shape.

@section kwk_performance_measuring Measuring what you wrote

KIWAKU ships no benchmark harness or profiler integration. The performance properties
above are *structural* (view-vs-table allocation, static-vs-runtime extents,
no-copy slicing), so the practical measurement loop is to compare two spellings side by
side:

- **Time a whole pass**: build each variant, run the same loop over `->size()` reads,
  and measure wall time with `std::chrono` (compile in `-O2`/`-O3`).
- **Watch the object size** with `sizeof(collection)` — it tells you whether storage is
  inline (static shapes), borrowed (a pointer), or heap-owned (a handle).
- **Compare borrow vs own**: the same computation expressed with `view` over a caller
  buffer versus `table` copies shows the allocation cost directly.
- **Check the assembly** for an element-wise loop (e.g. with `-S` or a
  godbolt-style view) — because access is inline, you should see straight loads/stores
  rather than calls.

Because there is no interpreter, "vectorizing" is mostly a matter of choosing
compile-time geometry and borrow semantics; measure those choices rather than hunting
for a scalar-vs-batch switch.

**/