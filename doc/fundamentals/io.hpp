#error This file is for documentation only - DO NOT INCLUDE
/**

@page kwk_io I/O with KIWAKU

NumPy has rich serialization (`np.save`/`np.load`, `savetxt`, NEP-18 style text) and a
whole-array printer. KIWAKU's I/O surface is deliberately narrow: the *descriptors* of a
collection — its shape, stride, kind, scheme and storage blob — stream out through
`operator<<`, but there is **no whole-collection value printer and no file/stream
serialization**. This page documents what prints, and gives the explicit pattern for the
rest.

@section kwk_io_printing Printing containers

Every *structure* of a collection has a streaming `operator<<`, so you can inspect the
metadata of `view`/`table` directly:

```cpp
#include <kiwaku/kiwaku.hpp>
#include <iostream>
using namespace kwk::literals;

float buf[6] = {0, 1, 2, 3, 4, 5};
auto  v     = kwk::view(kwk::source = buf, kwk::shape{2_c, 3_c});

std::cout << v->shape()  << '\n';   // the extents, e.g. `shape(2,3)`
std::cout << v->stride() << '\n';   // the memory strides between consecutive elements
std::cout << v->kind()   << '\n';   // the element kind, e.g. `Kind : float`
std::cout << v->scheme() << '\n';   // the per-field byte offsets / memory scheme
```

These stream their human-readable form because the underlying types (`shape`, `stride`,
`kind_option`, `scheme`, `blob`) each define `operator<<`. This is the most reliable way
to introspect what a collection actually is at run time — the layout, the extents and the
element type.

**There is no whole-collection printer.** `std::cout << v` (the array values, NumPy
style) does **not** compile: a `collection` has no `operator<<`. To print array contents,
walk the shape with a plain loop or fold:

```cpp
for (std::size_t i = 0; i < v->size(); ++i)
  std::cout << v[i] << (i + 1 < v->size() ? ", " : "");
std::cout << '\n';
```

For a multi-dimensional space, nest over the shape (or iterate the indices produced by
`->indexes(dim)` along each axis); for a product (SoA) kind, stream each field fibre via
`kwk::get<I>(v)` separately, since the record is not a contiguous cell to print whole.

@section kwk_io_streams Reading and writing streams

**KIWAKU has no file or stream serialization** — there is no analogue of `np.save` /
`np.load` / `savetxt`, and no way to round-trip a collection to a byte stream or text
file in one call. Storage is either borrowed from a caller-provided buffer or owned
inside the `table`; the library never opens files on your behalf.

What you can rely on:
- **Dump raw memory** — `->data()` returns a pointer to the storage, and `->size()` *
  `->kind()->itemsize` (or `->cosize()`) gives its byte length, so a caller can write the
  raw bytes to a file or socket:

```cpp
auto const* p = v.data();
std::size_t n_bytes = v->cosize() * v->kind()->itemsize;
// e.g. std::ofstream("out.bin").write(static_cast<char const*>(p), n_bytes);
```

- **Read raw memory back** — build a `view` (borrow) or `table` (own) over a buffer you
  filled, choosing the same shape and `kwk::as<T>` kind:

```cpp
float backing[6] = {};
auto  loaded     = kwk::view(kwk::source = backing, kwk::shape{2_c, 3_c});
// after filling `backing`, `loaded` is a ready 2x3 collection
```

The raw-memory round trip is the supported substitute for binary `np.save`/`np.load`;
there is no text/CSV writer, so formatting records for output is left to an explicit
loop (as above) — which also makes the byte layout (`itemsize` and stride) deliberate
rather than hidden behind a format string.

**/