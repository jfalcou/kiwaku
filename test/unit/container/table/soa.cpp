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

TTS_CASE("Build a product-kind (SoA) table from a struct-of-pointers source")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
  std::array<double, 4> ys = {10., 20., 30., 40.};

  auto v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{4_c}
                    , kwk::as<K>());

  TTS_EQUAL(v.ndim, 1);
  TTS_EQUAL(v->size(), 4);
  TTS_EQUAL(v->kind(), (kwk::as<K>()));

  // A product kind is an owning table: the fresh region is at least as large as the sum of the two
  // SoA fibers (4 floats + 4 doubles), and the object no longer aliases the source arrays.
  TTS_EQUAL(sizeof(v) >= (4 * sizeof(float) + 4 * sizeof(double)), true);

  // Per-field views over the SoA fibers.
  auto fx = kwk::get<0>(v);
  auto fy = kwk::get<1>(v);
  TTS_EQUAL(fx->kind(), (kwk::as<float>()));
  TTS_EQUAL(fy->kind(), (kwk::as<double>()));

  for (int k = 0; k < 4; ++k)
  {
    TTS_EQUAL(fx[k], xs[static_cast<std::size_t>(k)]);
    TTS_EQUAL(fy[k], ys[static_cast<std::size_t>(k)]);
    TTS_EQUAL(kwk::get<0>(v)[k], xs[static_cast<std::size_t>(k)]);
  }
};

TTS_CASE("Product-kind field views alias the parent storage")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
  std::array<double, 4> ys = {10., 20., 30., 40.};

  auto v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{4_c}
                    , kwk::as<K>());

  // Writing through a field view is visible through the parent's other field view.
  kwk::get<0>(v)[1] = 111.f;
  kwk::get<1>(v)[2] = 222.;
  TTS_EQUAL(kwk::get<0>(v)[1], 111.f);
  TTS_EQUAL(kwk::get<1>(v)[2], 222.);
  TTS_EQUAL(kwk::get<0>(v)[0], 1.f);
  TTS_EQUAL(kwk::get<0>(v)[3], 4.f);
};

TTS_CASE("Build a 2D product-kind (SoA) table")
{
  using namespace kwk::literals;
  using K = kumi::tuple<int, double>;

  int    ai[6]  = {1, 2, 3, 4, 5, 6};
  double ad[6]  = {10, 20, 30, 40, 50, 60};

  auto m = kwk::table(kwk::source = kumi::tuple{ai, ad}
                    , kwk::shape{2_c, 3_c}
                    , kwk::as<K>());

  TTS_EQUAL(m.ndim, 2);
  auto g0 = kwk::get<0>(m);
  auto g1 = kwk::get<1>(m);
  TTS_EQUAL(g0.ndim, 2);
  TTS_EQUAL(g1.ndim, 2);

  int idx = 0;
  for (int i = 0; i < 2; ++i)
    for (int j = 0; j < 3; ++j)
    {
      TTS_EQUAL((g0[i, j]), ai[idx]);
      TTS_EQUAL((g1[i, j]), ad[idx]);
      ++idx;
    }
};

TTS_CASE("Build a dynamic product-kind (SoA) table from a runtime shape")
{
  using K = kumi::tuple<float, double>;

  std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
  std::array<double, 4> ys = {10., 20., 30., 40.};
  int n = 4;

  auto v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{n}
                    , kwk::as<K>());

  TTS_EQUAL(v->size(), n);
  auto fx = kwk::get<0>(v);
  auto fy = kwk::get<1>(v);
  for (int k = 0; k < n; ++k)
  {
    TTS_EQUAL(fx[k], xs[static_cast<std::size_t>(k)]);
    TTS_EQUAL(fy[k], ys[static_cast<std::size_t>(k)]);
  }
  TTS_EQUAL(fx[2], 3.f);
};

TTS_CASE("Build a product-kind (SoA) table from an array-of-structures source (vector<tuple>)")
{
  using K = kumi::tuple<float, double>;

  std::vector<kumi::tuple<float, double>> aos = {
    {1.f, 10.}, {2.f, 20.}, {3.f, 30.}, {4.f, 40.}
  };

  // By default a product-kind table owned from an AoS range source is transposed to SoA: each
  // field occupies its own fiber in the fresh owning region.
  auto v = kwk::table(kwk::source = aos, kwk::as<K>());

  TTS_EQUAL(v.ndim, 1);
  TTS_EQUAL(v->size(), 4);
  TTS_EQUAL(v->kind(), (kwk::as<K>()));

  auto fx = kwk::get<0>(v);
  auto fy = kwk::get<1>(v);
  TTS_EQUAL(fx->kind(), (kwk::as<float>()));
  TTS_EQUAL(fy->kind(), (kwk::as<double>()));

  for (std::size_t k = 0; k < aos.size(); ++k)
  {
    TTS_EQUAL(fx[k], kumi::get<0>(aos[k]));
    TTS_EQUAL(fy[k], kumi::get<1>(aos[k]));
  }

  // Write-through aliasing still holds across the transposed fibers.
  kwk::get<0>(v)[1] = 111.f;
  kwk::get<1>(v)[2] = 222.;
  TTS_EQUAL(kwk::get<0>(v)[1], 111.f);
  TTS_EQUAL(kwk::get<1>(v)[2], 222.);
  TTS_EQUAL(kwk::get<0>(v)[0], 1.f);
};

TTS_CASE("Build a product-kind (SoA) table from a std::array of records (AoS)")
{
  using K = kumi::tuple<int, double>;

  std::array<kumi::tuple<int, double>, 3> aos = {
    kumi::tuple{1, 10.}, kumi::tuple{2, 20.}, kumi::tuple{3, 30.}
  };

  auto v = kwk::table(kwk::source = aos, kwk::as<K>());

  TTS_EQUAL(v->size(), 3);
  auto gi = kwk::get<0>(v);
  auto gd = kwk::get<1>(v);
  for (std::size_t k = 0; k < aos.size(); ++k)
  {
    TTS_EQUAL(gi[k], kumi::get<0>(aos[k]));
    TTS_EQUAL(gd[k], kumi::get<1>(aos[k]));
  }
};

TTS_CASE("A vector<tuple> source auto-converts to SoA even without an explicit as<K>() kind")
{
  using K = kumi::tuple<float, double>;

  std::vector<K> aos = {{1.f, 10.}, {2.f, 20.}, {3.f, 30.}, {4.f, 40.}};

  // No kwk::as<K>() is passed: the product kind is auto-deduced from the tuple element type of
  // the range source, and the owning table transposes the interleaved records to SoA fibers.
  auto v = kwk::table(kwk::source = aos);

  TTS_EQUAL(v.ndim, 1);
  TTS_EQUAL(v->size(), 4);
  TTS_TYPE_IS(std::decay_t<decltype(v->kind())>::element_type, K);

  auto fx = kwk::get<0>(v);
  auto fy = kwk::get<1>(v);
  TTS_EQUAL(fx->kind(), (kwk::as<float>()));
  TTS_EQUAL(fy->kind(), (kwk::as<double>()));

  for (std::size_t k = 0; k < aos.size(); ++k)
  {
    TTS_EQUAL(fx[k], kumi::get<0>(aos[k]));
    TTS_EQUAL(fy[k], kumi::get<1>(aos[k]));
  }

  // Write-through aliasing across the transposed fibers of the auto-deduced product kind.
  kwk::get<0>(v)[1] = 111.f;
  kwk::get<1>(v)[2] = 222.;
  TTS_EQUAL(kwk::get<0>(v)[1], 111.f);
  TTS_EQUAL(kwk::get<1>(v)[2], 222.);
};

TTS_CASE("SoA slicing re-bases each field fiber, not the whole record (audit J7)")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
  std::array<double, 4> ys = {10., 20., 30., 40.};

  auto v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{4_c}
                    , kwk::as<K>());

  // Slice out elements [2, 4) : the float fiber shifts by 2*sizeof(float) and the double fiber by
  // 2*sizeof(double). A single re-point by 2*sizeof(record) would misplace the wider double fiber.
  auto s = v[kwk::slicer{2, 4, 1}];
  TTS_EQUAL(s->size(), 2);
  auto sf = kwk::get<0>(s);
  auto sd = kwk::get<1>(s);
  TTS_EQUAL(sf[0], 3.f);
  TTS_EQUAL(sf[1], 4.f);
  TTS_EQUAL(sd[0], 30.);
  TTS_EQUAL(sd[1], 40.);

  // The parent is untouched by the slice.
  TTS_EQUAL(kwk::get<0>(v)[0], 1.f);
  TTS_EQUAL(kwk::get<1>(v)[3], 40.);
};

TTS_CASE("SoA tiling re-bases each field fiber (audit J7)")
{
  using namespace kwk::literals;
  using K = kumi::tuple<int, double>;

  int    ai[6] = {1, 2, 3, 4, 5, 6};
  double ad[6] = {10., 20., 30., 40., 50., 60.};

  auto m = kwk::table(kwk::source = kumi::tuple{ai, ad}
                    , kwk::shape{2_c, 3_c}
                    , kwk::as<K>());

  // m[1] addresses the second row: the per-field fibers each advance by 3 scalars.
  auto r = m[1];
  TTS_EQUAL(r->shape(), (kwk::shape{3_c}));
  auto rf = kwk::get<0>(r);
  auto rd = kwk::get<1>(r);
  for (int j = 0; j < 3; ++j)
  {
    TTS_EQUAL(rf[j], ai[3 + j]);
    TTS_EQUAL(rd[j], ad[3 + j]);
  }

  // m[0] is the first row, unshifted.
  auto r0 = m[0];
  TTS_EQUAL(kwk::get<0>(r0)[1], 2);
  TTS_EQUAL(kwk::get<1>(r0)[2], 30.);
};

TTS_CASE("Heap-owned SoA slicing/tiling borrows (no copy) and keeps write-through (audit M-F)")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  // --- Slice on a 1D heap-owned product table ------------------------------------------------
  std::array<float, 4>  xs = {1.f, 2.f, 3.f, 4.f};
  std::array<double, 4> ys = {10., 20., 30., 40.};
  int n = 4;

  // Runtime shape => heap-owned product table (move-only backend). Slicing must BORROW the parent's
  // heap data (re-point to data()) and re-base each field fiber via the scheme — never pass the
  // owning heap blob by value (move-only => compile error before this fix).
  auto v = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{n}
                    , kwk::as<K>());
  auto s = v[kwk::slicer{2, 4, 1}];
  TTS_EQUAL(s->size(), 2);
  auto sf = kwk::get<0>(s);
  auto sd = kwk::get<1>(s);
  TTS_EQUAL(sf[0], 3.f);
  TTS_EQUAL(sf[1], 4.f);
  TTS_EQUAL(sd[0], 30.);
  TTS_EQUAL(sd[1], 40.);

  // Write-through aliasing into the parent's heap region (borrowed, not copied).
  sf[1] = 444.f;
  TTS_EQUAL(kwk::get<0>(v)[3], 444.f);
  TTS_EQUAL(kwk::get<0>(v)[0], 1.f);

  // --- Tile (partial index) on a 2D heap-owned product table ----------------------------------
  std::array<float, 6>  ax = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
  std::array<double, 6> ay = {10., 20., 30., 40., 50., 60.};
  int rows = 2, cols = 3;

  auto m = kwk::table(kwk::source = kumi::tuple{ax.data(), ay.data()}
                    , kwk::shape{rows, cols}
                    , kwk::as<K>());

  // m[1] -> the second row, each field fiber re-based by 3 scalars.
  auto r = m[1];
  TTS_EQUAL(r->size(), 3);
  TTS_EQUAL(kwk::get<0>(r)[0], 4.f);
  TTS_EQUAL(kwk::get<1>(r)[2], 60.);
  TTS_EQUAL(kwk::get<0>(r)[2], 6.f);

  // Write-through from the tiled field view reaches the parent (borrowed).
  kwk::get<1>(r)[1] = 555.;
  TTS_EQUAL((kwk::get<1>(m)[1, 1]), 555.);
};
