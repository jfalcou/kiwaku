//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>
#include <kumi/tuple.hpp>
#include <array>
#include <vector>

//==================================================================================================
// M4 : option-behavior matrix — every option the builder accepts inside a `kwk::table`, each
// verified for the type it selects, the layout it describes, its read/write path, and the
// shape()/size()/ndim it reports. The option set is exercised in the *owning* `table` factory
// (which appends `deduce_allocator`), plus explicit-allocator variants.
//==================================================================================================

//--- source forms : every memory-backed source spelling yields the right element type & layout ---
TTS_CASE("M4 - source matrix : C-array pointer / std::array / contiguous range in an owning table")
{
  using namespace kwk::literals;

  float carr[4] = {1.f, 2.f, 3.f, 4.f};
  std::array<float, 4> sarr = {10.f, 20.f, 30.f, 40.f};
  std::vector<float> vec = {100.f, 200.f, 300.f, 400.f};

  // C-array pointer -> float kind, 1D, size 4.
  auto tp = kwk::table(kwk::source = carr, kwk::shape{4_c});
  TTS_EQUAL(tp->ndim, 1);
  TTS_EQUAL(tp->size(), 4);
  TTS_EQUAL(tp->shape(), (kwk::shape{4_c}));
  TTS_TYPE_IS(std::decay_t<decltype(tp->kind())>::element_type, float);
  TTS_EQUAL(tp[0], 1.f);
  TTS_EQUAL(tp[3], 4.f);
  tp[1] = 7.f;                 // owning copy write-through into the fresh region
  TTS_EQUAL(tp[1], 7.f);

  // std::array source.
  auto ta = kwk::table(kwk::source = sarr, kwk::shape{4_c});
  TTS_TYPE_IS(std::decay_t<decltype(ta->kind())>::element_type, float);
  TTS_EQUAL(ta->size(), 4);
  TTS_EQUAL(ta[2], 30.f);

  // Contiguous range (vector) source.
  auto tv = kwk::table(kwk::source = vec, kwk::shape{4_c});
  TTS_TYPE_IS(std::decay_t<decltype(tv->kind())>::element_type, float);
  TTS_EQUAL(tv->size(), 4);
  TTS_EQUAL(tv[3], 400.f);
};

TTS_CASE("M4 - of_shape drives the extent; an absent source leaves the wildcard single-element grid")
{
  using namespace kwk::literals;

  // Explicit of_shape wins over the source's implicit shape and over the wildcard default.
  float ref[6] = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f};
  auto t = kwk::table(kwk::source = ref, kwk::of_shape = kwk::shape{2_c, 3_c});
  TTS_EQUAL(t->ndim, 2);
  TTS_EQUAL(t->size(), 6);
  TTS_EQUAL(t->shape(), (kwk::shape{2_c, 3_c}));
  auto v12 = t[1, 2];
  TTS_EQUAL(v12, 5.f);

  // Any owning table reports row-major strides: flat index = i*3 + j.
  for (auto i : {0, 1}) for (auto j : {0, 1, 2})
  {
    auto vij = t[i, j];
    TTS_EQUAL(vij, ref[i * 3 + j]);
  }
};

//--- kind opt : scalar precision + numeric width selection ---------------------------------------
TTS_CASE("M4 - kind option : scalar real + ordinal kinds are selected exactly")
{
  using namespace kwk::literals;

  std::array<double, 3> dref = {1.5, 2.5, 3.5};
  auto td = kwk::table(kwk::source = dref, kwk::shape{3_c}, kwk::as<double>());
  TTS_TYPE_IS(std::decay_t<decltype(td->kind())>::element_type, double);
  TTS_EQUAL(td[1], 2.5);

  std::array<std::int16_t, 3> iref = {7, 8, 9};
  auto ti = kwk::table(kwk::source = iref, kwk::shape{3_c}, kwk::as<std::int16_t>());
  TTS_TYPE_IS(std::decay_t<decltype(ti->kind())>::element_type, std::int16_t);
  TTS_EQUAL(ti[2], (std::int16_t)9);
};

TTS_CASE("M4 - kind option : nested product kind (SoA) from an array-of-structures source")
{
  using K = kumi::tuple<float, double>;

  std::vector<K> aos = {{1.f, 10.}, {2.f, 20.}, {3.f, 30.}};
  auto v = kwk::table(kwk::source = aos, kwk::as<K>());

  TTS_TYPE_IS(std::decay_t<decltype(v->kind())>::element_type, K);
  TTS_EQUAL(v->size(), 3);
  TTS_EQUAL(v->ndim, 1);

  // SoA transpose : each field is its own borrowed fiber with correct values.
  auto fx = kwk::get<0>(v);
  auto fy = kwk::get<1>(v);
  TTS_TYPE_IS(std::decay_t<decltype(fx->kind())>::element_type, float);
  TTS_TYPE_IS(std::decay_t<decltype(fy->kind())>::element_type, double);
  TTS_EQUAL(fx[0], 1.f); TTS_EQUAL(fx[2], 3.f);
  TTS_EQUAL(fy[0], 10.); TTS_EQUAL(fy[2], 30.);

  // Write-through aliasing back into the owning SoA region.
  kwk::get<0>(v)[1] = 99.f;
  TTS_EQUAL(kwk::get<0>(v)[1], 99.f);
};

//--- of_stride + storage_order : explicit layout control inside the table ------------------------
TTS_CASE("M4 - of_stride : an explicit stride overrides the order-derived row-major layout")
{
  using namespace kwk::literals;

  // A 2x2 region addressed with an inverted stride : elements appear in reverse order.
  std::array<int, 4> ref = {1, 2, 3, 4};
  auto t = kwk::table(kwk::source = ref, kwk::shape{2_c, 2_c},
                      kwk::of_stride = kwk::stride{1_c, 2_c});
  TTS_EQUAL(t->stride(), (kwk::stride{1_c, 2_c}));
  // stride (1, 2): flat = i*1 + j*2  =>  (0,0)->1, (1,0)->2, (0,1)->3, (1,1)->4.
  auto v00 = t[0, 0];
  auto v10 = t[1, 0];
  auto v01 = t[0, 1];
  auto v11 = t[1, 1];
  TTS_EQUAL(v00, 1);
  TTS_EQUAL(v10, 2);
  TTS_EQUAL(v01, 3);
  TTS_EQUAL(v11, 4);
};

TTS_CASE("M4 - storage_order : column-major layout inside the table (no explicit stride)")
{
  using namespace kwk::literals;

  std::array<int, 4> ref = {1, 2, 3, 4};
  auto rc = kwk::table(kwk::source = ref, kwk::shape{2_c, 2_c}, kwk::storage_order = kwk::column_major_order);
  TTS_EQUAL(rc->stride(), (kwk::to_stride(kwk::shape{2_c, 2_c}, kwk::column_major_order)));
  // Column-major stride (1, 2) addresses the same buffer in the transposed order.
  auto c00 = rc[0, 0];
  auto c10 = rc[1, 0];
  auto c01 = rc[0, 1];
  auto c11 = rc[1, 1];
  TTS_EQUAL(c00, 1);
  TTS_EQUAL(c10, 2);
  TTS_EQUAL(c01, 3);
  TTS_EQUAL(c11, 4);
};

//--- boundary condition options ------------------------------------------------------------------
TTS_CASE("M4 - boundary options : clamp / tile / mirror inside an owning table")
{
  using namespace kwk::literals;
  std::array<float, 4> ref = {10.f, 20.f, 30.f, 40.f};

  auto cl = kwk::table(kwk::source = ref, kwk::shape{4_c}, kwk::clamp);
  TTS_EQUAL(cl[-1], 10.f);
  TTS_EQUAL(cl[4], 40.f);

  auto tl = kwk::table(kwk::source = ref, kwk::shape{4_c}, kwk::tile);
  TTS_EQUAL(tl[4], 10.f);
  TTS_EQUAL(tl[5], 20.f);
  TTS_EQUAL(tl[-1], 40.f);

  auto mr = kwk::table(kwk::source = ref, kwk::shape{4_c}, kwk::mirror);
  TTS_EQUAL(mr[4], 30.f);  // 0,1,2,3,2,1,... period = 2*(4-1) = 6
  TTS_EQUAL(mr[5], 20.f);
};

//--- interpolator option -------------------------------------------------------------------------
TTS_CASE("M4 - interpolator option : linear blends neighbouring samples")
{
  using namespace kwk::literals;
  std::array<float, 5> ref = {0.f, 1.f, 2.f, 3.f, 4.f};

  auto lin = kwk::table(kwk::source = ref, kwk::shape{5_c}, kwk::linear);
  TTS_EQUAL(lin[2.5], 2.5f);
  TTS_EQUAL(lin[1.75], 1.75f);

  auto nrst = kwk::table(kwk::source = ref, kwk::shape{5_c}, kwk::nearest);
  TTS_EQUAL(nrst[2.5], 3.f);  // half away from zero
  TTS_EQUAL(nrst[1.4], 1.f);
};

//--- base_index option ---------------------------------------------------------------------------
TTS_CASE("M4 - base_index : c_index (0-based) is identity; fortran_index shifts the origin")
{
  using namespace kwk::literals;
  float ref[5] = {10.f, 20.f, 30.f, 40.f, 50.f};

  auto tc = kwk::table(kwk::source = ref, kwk::shape{5_c}, kwk::c_index);
  TTS_EQUAL(tc[0], 10.f);
  TTS_EQUAL(tc[4], 50.f);

  auto tf = kwk::table(kwk::source = ref, kwk::shape{5_c}, kwk::fortran_index);
  TTS_EQUAL(tf[1], 10.f);
  TTS_EQUAL(tf[5], 50.f);
};

//--- coordinate option ---------------------------------------------------------------------------
TTS_CASE("M4 - coordinate option : canonical grid coordinates")
{
  using namespace kwk::literals;
  std::array<float, 4> ref = {1.f, 2.f, 3.f, 4.f};

  auto cn = kwk::table(kwk::source = ref, kwk::shape{4_c}, kwk::coordinate = kwk::canonical);
  TTS_EQUAL(cn[0], 1.f);
  TTS_EQUAL(cn[3], 4.f);
};

//--- generator option (function-defined space, no storage) ---------------------------------------
TTS_CASE("M4 - generator option : function-defined space, shape derived, no allocation")
{
  using namespace kwk::literals;

  // A pure position-dependent generator : value at a flat index == its coordinate.
  constexpr auto ramp = [](auto... xs) { return (0 + ... + xs); };
  auto g = kwk::table(kwk::generator = ramp, kwk::shape{4_c});
  TTS_EQUAL(g->ndim, 1);
  TTS_EQUAL(g->size(), 4);
  TTS_EQUAL(static_cast<int>(g[0]), 0);
  TTS_EQUAL(static_cast<int>(g[3]), 3);

  // The constant_blob carries no heap storage.
  TTS_EQUAL(sizeof(g), 1u);
};

//--- allocator + deduce_allocator options --------------------------------------------------------
TTS_CASE("M4 - allocator : explicit heap / stack backends, and deduce_allocator via the table factory")
{
  using namespace kwk::literals;

  std::array<float, 4> ref = {1.f, 2.f, 3.f, 4.f};

  // Explicit allocator option (heap) : an owning heap-blob region for dynamic geometry (the only
  // spellings that satisfy kwk::concepts::allocator carry allocate/deallocate, e.g. `kwk::heap`;
  // the `kwk::stack<...>` tag is a size/alignment *hint*, not a runtime allocator, and is
  // deliberately rejected as an option).
  auto th = kwk::table(kwk::source = ref, kwk::shape{4}, kwk::allocator = kwk::heap);
  TTS_EQUAL(sizeof(th), (3 * sizeof(void*)));   // heap-blob backend
  TTS_EQUAL(th->size(), 4);
  TTS_EQUAL(th[0], 1.f);
  th[1] = 22.f;
  TTS_EQUAL(th[1], 22.f);

  // deduce_allocator (what the `table` factory appends) with fully-static geometry selects the
  // inline stack backend : the table is byte-for-byte its static region (4 * sizeof(float)) and
  // remains writable (owning, non-const accessor).
  auto t_static = kwk::table(kwk::source = ref, kwk::shape{4_c});
  TTS_EQUAL(sizeof(t_static), 4 * sizeof(float));
  TTS_EQUAL(t_static->size(), 4);
  t_static[0] = 5.f;
  TTS_EQUAL(t_static[0], 5.f);
  TTS_EQUAL(t_static[3], 4.f);

  // deduce_allocator with dynamic geometry selects the heap backend (3 * sizeof(void*)).
  auto t_dyn = kwk::table(kwk::source = ref, kwk::shape{4});
  TTS_EQUAL(sizeof(t_dyn), (3 * sizeof(void*)));
  TTS_EQUAL(t_dyn[2], 3.f);
  t_dyn[0] = 9.f;
  TTS_EQUAL(t_dyn[0], 9.f);
};

//--- combined : several options compose in a single owning table ---------------------------------
TTS_CASE("M4 - combined options compose (source + of_shape + kind + boundary + base_index + linear)")
{
  using namespace kwk::literals;
  std::array<float, 6> ref = {0.f, 1.f, 2.f, 3.f, 4.f, 5.f};

  auto t = kwk::table(kwk::source = ref,
                      kwk::of_shape = kwk::shape{2_c, 3_c},
                      kwk::as<float>(),
                      kwk::tile,
                      kwk::fortran_index,
                      kwk::linear);

  TTS_EQUAL(t->ndim, 2);
  TTS_EQUAL(t->size(), 6);
  TTS_EQUAL(t->shape(), (kwk::shape{2_c, 3_c}));
  TTS_TYPE_IS(std::decay_t<decltype(t->kind())>::element_type, float);

  // 1-based origin : logical (i,j) -> 0-based (i-1, j-1) -> flat (i-1)*3 + (j-1). At integer
  // corners a `linear` interpolator returns the exact vertex, so these still hold.
  auto c11 = t[1, 1];
  auto c13 = t[1, 3];
  auto c23 = t[2, 3];
  TTS_EQUAL(c11, 0.f);
  TTS_EQUAL(c13, 2.f);
  TTS_EQUAL(c23, 5.f);

  // tiling wraps the *physical* row index : logical row 3 -> physical row 2 -> row 0.
  auto c31 = t[3, 1];
  TTS_EQUAL(c31, 0.f);
  TTS_EQUAL(c31, c11);
};
