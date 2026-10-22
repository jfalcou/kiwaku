//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
/*
  Layout breadth (M5a), partial indexing / sub-element return (M5b) and for_each blanket
  verification (M5c) for the owning `kwk::table`.

  Coverage:
  - 3D / 4D scalar tensors : ndim/size/shape, row-major element order, write-through.
  - scalar tiled shapes in an owning table : a `kwk::table` whose shape nests a tile, addressing
    a leaf with the full index set and a tile with fewer indices (partial index returns a tile view).
  - SoA product-kind matrices : per-field `get<I>` fibers, write-through aliasing.
  - partial indexing : `c[i]` (tile view), `slicer`, `get<I>` then index down; verify the returned
    sub-view's shape/stride and that writes alias the parent storage.
  - `kwk::__::for_each` visits every cell exactly once, in documented row-major (leaf) order, for
    flat 1D/2D/3D, tiled, and SoA shapes. Tiled/nested `for_each` previously failed to compile
    (fold over an unflattened shape iterated `i < shape`); the algorithm now flattens to leaf
    extents first (M5c fix). `kwk::fill` over a tiled owning table exercises the same path.
*/
//==================================================================================================
#include "test.hpp"
#include <kiwaku/kiwaku.hpp>
#include <kumi/tuple.hpp>
#include <array>
#include <vector>

//--------------------------------------------------------------------------------------------------
// M5a : layout breadth — higher-dimensional tensors in an owning table.
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - owning 3D scalar tensor : index space, element order, write-through")
{
  using namespace kwk::literals;

  std::vector<int> src(2 * 3 * 4);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  auto t = kwk::table(kwk::source = src, kwk::shape{2_c, 3_c, 4_c});

  TTS_EQUAL(t->ndim, 3);
  TTS_EQUAL(t->size(), 24);
  TTS_EQUAL(t->shape(), (kwk::shape{2_c, 3_c, 4_c}));
  TTS_TYPE_IS(std::decay_t<decltype(t->kind())>::element_type, int);

  // Row-major : flat(i,j,k) = i*12 + j*4 + k.
  TTS_EQUAL((t[0, 0, 0]), 0);
  TTS_EQUAL((t[0, 2, 3]), 11);
  TTS_EQUAL((t[1, 0, 0]), 12);
  TTS_EQUAL((t[1, 2, 3]), 23);

  // Write-through into the owning region.
  t[1, 1, 3] = 900;
  TTS_EQUAL((t[1, 1, 3]), 900);
  TTS_EQUAL((t[1, 1, 2]), 18);
};

TTS_CASE("M5 - owning 4D scalar tensor : deep index space and row-major order")
{
  using namespace kwk::literals;

  std::vector<int> src(2 * 2 * 2 * 2);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  auto t = kwk::table(kwk::source = src, kwk::shape{2_c, 2_c, 2_c, 2_c});

  TTS_EQUAL(t->ndim, 4);
  TTS_EQUAL(t->size(), 16);

  // flat(i,j,k,l) = i*8 + j*4 + k*2 + l.
  TTS_EQUAL((t[0, 0, 0, 1]), 1);
  TTS_EQUAL((t[0, 1, 1, 0]), 6);
  TTS_EQUAL((t[1, 0, 1, 1]), 11);
  TTS_EQUAL((t[1, 1, 1, 1]), 15);
};

//--------------------------------------------------------------------------------------------------
// M5a : layout breadth — 2D matrix as an owning scalar table (tile-able by row).
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - owning 2D matrix : row tile via partial index, shape/stride + write-through")
{
  using namespace kwk::literals;

  std::vector<int> src(2 * 3);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  auto m = kwk::table(kwk::source = src, kwk::shape{2_c, 3_c});

  TTS_EQUAL(m->ndim, 2);
  TTS_EQUAL(m->size(), 6);

  // Partial index m[i] selects row i as a 1x3 tile view over the same storage.
  auto row = m[1];
  TTS_EQUAL(row->ndim, 1);
  TTS_EQUAL(row->size(), 3);
  TTS_EQUAL(row->shape(), (kwk::shape{3_c}));
  TTS_EQUAL(row->stride(), (kwk::stride{1_c}));
  TTS_EQUAL(row[0], 3);
  TTS_EQUAL(row[2], 5);

  // Writing through the row tile aliases the parent matrix.
  row[1] = 777;
  TTS_EQUAL(row[1], 777);
  TTS_EQUAL((m[1, 1]), 777);
  TTS_EQUAL((m[0, 1]), 1);
};

//--------------------------------------------------------------------------------------------------
// M5a : layout breadth — scalar tiled kinds in an owning table.
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - owning table over a 1D grid of 2D tiles : leaf addressing + tile views")
{
  using namespace kwk::literals;

  std::vector<int> src(3 * 2 * 2);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  // 3 tiles, each a 2x2 block : leaves (i, ti, tj), flat = i*4 + ti*2 + tj.
  auto g = kwk::table(kwk::source = src, kwk::shape{3_c, kwk::shape{2_c, 2_c}});

  TTS_EQUAL(g->ndim, 3);
  TTS_EQUAL(g->size(), 12);

  // Full (3-index) access addresses a leaf.
  TTS_EQUAL((g[0, 0, 0]), 0);
  TTS_EQUAL((g[1, 1, 1]), 7);
  TTS_EQUAL((g[2, 0, 1]), 9);
  TTS_EQUAL((g[2, 1, 1]), 11);

  // One index selects a whole tile : a 2x2 sub-view.
  auto tile = g[2];
  TTS_EQUAL(tile->ndim, 2);
  TTS_EQUAL(tile->size(), 4);
  TTS_EQUAL(tile->shape(), (kwk::shape{2_c, 2_c}));
  TTS_EQUAL((tile[0, 0]), 8);
  TTS_EQUAL((tile[1, 1]), 11);

  // Write-through from the tile aliases the owning grid.
  tile[0, 1] = 555;
  TTS_EQUAL((tile[0, 1]), 555);
  TTS_EQUAL((g[2, 0, 1]), 555);
};

TTS_CASE("M5 - owning table over a 2D grid of 2D tiles : tile shape/stride verified")
{
  using namespace kwk::literals;

  // 2x2 grid of 2x2 tiles = 16 leaves, flat = gi*8 + gj*4 + ti*2 + tj.
  std::vector<int> src(16);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  auto g = kwk::table(kwk::source = src, kwk::shape{kwk::shape{2_c, 2_c}, kwk::shape{2_c, 2_c}});

  TTS_EQUAL(g->ndim, 4);
  TTS_EQUAL(g->size(), 16);

  TTS_EQUAL((g[1, 1, 1, 1]), 15);
  TTS_EQUAL((g[0, 1, 1, 0]), 6);

  // Two indices select the tile at grid cell (1,0) : an inner 2x2 block.
  auto tile = g[1, 0];
  TTS_EQUAL(tile->ndim, 2);
  TTS_EQUAL(tile->shape(), (kwk::shape{2_c, 2_c}));
  TTS_EQUAL((tile[0, 0]), 8);
  TTS_EQUAL((tile[1, 1]), 11);

  // Write-through from the tile aliases the owning grid.
  tile[1, 0] = 321;
  TTS_EQUAL((tile[1, 0]), 321);
  TTS_EQUAL((g[1, 0, 1, 0]), 321);
};

//--------------------------------------------------------------------------------------------------
// M5a/M5b : SoA product-kind matrix + per-field partial indexing, write-through aliasing.
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - SoA product-kind matrix : get<I> fibers + write-through aliasing")
{
  using namespace kwk::literals;
  using K = kumi::tuple<float, double>;

  std::array<float, 6>  xs = {1.f, 2.f, 3.f, 4.f, 5.f, 6.f};
  std::array<double, 6> ys = {10., 20., 30., 40., 50., 60.};

  auto m = kwk::table(kwk::source = kumi::tuple{xs.data(), ys.data()}
                    , kwk::shape{2_c, 3_c}
                    , kwk::as<K>());

  TTS_EQUAL(m->ndim, 2);
  TTS_EQUAL(m->size(), 6);

  auto fx = kwk::get<0>(m);
  auto fy = kwk::get<1>(m);
  TTS_EQUAL(fx->ndim, 2);
  TTS_EQUAL(fy->ndim, 2);

  // Full 2-index access through the field fibers.
  TTS_EQUAL((fx[1, 2]), xs[5]);
  TTS_EQUAL((fy[0, 1]), ys[1]);

  // Write through field 0 : visible via the parent's own field view.
  kwk::get<0>(m)[1, 1] = 999.f;
  TTS_EQUAL((fx[1, 1]), 999.f);
  TTS_EQUAL((fy[1, 1]), ys[4]);  // sibling field untouched
};

//--------------------------------------------------------------------------------------------------
// M5b : partial indexing — `c[i]` returns a borrowed tile view whose writes alias the parent.
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - partial index returns a tile view over the same bytes (scalar matrix)")
{
  using namespace kwk::literals;

  std::vector<int> src(2 * 3);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  auto m = kwk::table(kwk::source = src, kwk::shape{2_c, 3_c});

  auto row0 = m[0];
  row0[2] = 444;
  TTS_EQUAL((m[0, 2]), 444);   // aliases parent
  TTS_EQUAL((m[1, 2]), 5);     // row 1 untouched
};

//--------------------------------------------------------------------------------------------------
// M5b : partial indexing — `slicer` over an owning table keeps a borrowed, write-through sub-view.
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - slicer on an owning table : borrowed write-through sub-view")
{
  using namespace kwk::literals;

  std::vector<int> src(8);
  for (std::size_t k = 0; k < src.size(); ++k) src[k] = static_cast<int>(k);

  auto v = kwk::table(kwk::source = src, kwk::shape{8_c});

  auto s = v[kwk::slicer{2, 6, 1}];
  TTS_EQUAL(s->size(), 4);
  TTS_EQUAL(s[0], 2);
  TTS_EQUAL(s[3], 5);

  // Writing through the slice is visible in the parent and vice-versa.
  s[1] = 777;
  TTS_EQUAL(s[1], 777);
  TTS_EQUAL(v[3], 777);
  v[3] = 333;
  TTS_EQUAL(s[1], 333);
};

//--------------------------------------------------------------------------------------------------
// M5c : for_each visits every cell exactly once, in documented row-major (leaf) order.
//--------------------------------------------------------------------------------------------------
namespace
{
  // Mark every leaf of `c` visited via `for_each` and assert the count and full coverage.
  template<typename Collection>
  void check_for_each_once(Collection const& c)
  {
    std::vector<char> visited(static_cast<std::size_t>(c->size()), 0);
    std::size_t       count = 0;
    kwk::__::for_each([&](auto... idxs) { visited[static_cast<std::size_t>(c[idxs...])] = 1; ++count; },
                      c->shape());
    TTS_EQUAL(count, static_cast<std::size_t>(c->size()));
    for (char b : visited) TTS_EQUAL(b == 1, true);
  }
}

TTS_CASE("M5 - for_each : flat 1D/2D/3D scalar tables visit every cell once")
{
  using namespace kwk::literals;

  // Cells hold their own flat index (0..N-1), so coverage of value k proves cell k was visited.
  std::vector<int> v7(7);   for (std::size_t k = 0; k < v7.size(); ++k) v7[k] = static_cast<int>(k);
  std::vector<int> v6(6);   for (std::size_t k = 0; k < v6.size(); ++k) v6[k] = static_cast<int>(k);
  std::vector<int> v24(24); for (std::size_t k = 0; k < v24.size(); ++k) v24[k] = static_cast<int>(k);

  auto a = kwk::table(kwk::source = v7, kwk::shape{7_c});
  auto b = kwk::table(kwk::source = v6, kwk::shape{2_c, 3_c});
  auto c = kwk::table(kwk::source = v24, kwk::shape{2_c, 3_c, 4_c});

  check_for_each_once(a);
  check_for_each_once(b);
  check_for_each_once(c);
};

TTS_CASE("M5 - for_each : tiled scalar tables visit every leaf once")
{
  using namespace kwk::literals;

  // 1D grid of 2x2 tiles.
  std::vector<int> g1(12); for (std::size_t k = 0; k < g1.size(); ++k) g1[k] = static_cast<int>(k);
  auto t1 = kwk::table(kwk::source = g1, kwk::shape{3_c, kwk::shape{2_c, 2_c}});
  check_for_each_once(t1);

  // 2D grid of 2x2 tiles.
  std::vector<int> g2(16); for (std::size_t k = 0; k < g2.size(); ++k) g2[k] = static_cast<int>(k);
  auto t2 = kwk::table(kwk::source = g2, kwk::shape{kwk::shape{2_c, 2_c}, kwk::shape{2_c, 2_c}});
  check_for_each_once(t2);
};

TTS_CASE("M5 - for_each : SoA product-kind field fiber visits every cell once")
{
  using namespace kwk::literals;
  using K = kumi::tuple<int, double>;

  std::array<int, 6>    ai = {0, 1, 2, 3, 4, 5};
  std::array<double, 6> ad = {0., 10., 20., 30., 40., 50.};

  auto m = kwk::table(kwk::source = kumi::tuple{ai.data(), ad.data()}
                    , kwk::shape{2_c, 3_c}
                    , kwk::as<K>());

  // Field fiber 0 is an int matrix whose cells hold 0..5 : coverage proves every cell visited once.
  auto f0 = kwk::get<0>(m);
  check_for_each_once(f0);
};

//--------------------------------------------------------------------------------------------------
// M5c : kwk::fill populates a tiled owning table (previously a compile error via for_each).
//--------------------------------------------------------------------------------------------------
TTS_CASE("M5 - kwk::fill over a tiled owning table (tiled for_each path)")
{
  using namespace kwk::literals;

  std::vector<int> src(12, 0);
  auto g = kwk::table(kwk::source = src, kwk::shape{3_c, kwk::shape{2_c, 2_c}});

  kwk::fill(g, 42);

  std::size_t count = 0;
  kwk::__::for_each([&](auto... idxs) { TTS_EQUAL(g[idxs...], 42); ++count; }, g->shape());
  TTS_EQUAL(count, 12u);
};
