//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/kwk.hpp>
#include <algorithm>

TTS_CASE("Indexing Behavior")
{
  using kwk::_;
  using namespace kwk::literals;

  float ref[7] = {1, 2, 3, 4, 5, 6, 7};
  float const cref[7] = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7;
  auto v = kwk::view(kwk::source = ref, kwk::shape{sz});

  for (std::size_t i = 0; i < static_cast<std::size_t>(v.size()); ++i) TTS_EQUAL(v[i], ref[i]);
  TTS_EXPECT_COMPILES(v, { v[0] = 3.f; });

  auto w = kwk::view(kwk::source = cref, kwk::shape{sz});
  for (std::size_t i = 0; i < static_cast<std::size_t>(w.size()); ++i) TTS_EQUAL(w[i], cref[i]);
  TTS_EXPECT_NOT_COMPILES(w, { w[0] = 3.f; });

  auto sl1 = kwk::to_slicer(_);
  auto sl2 = kwk::to_slicer(2);
  auto sl3 = kwk::to_slicer(2_c);
  auto sl4 = kwk::slicer{2, 6, 2};
  auto sl5 = kwk::end / 2;

  auto vs1 = v[sl1];
  for (std::size_t i = 0; i < static_cast<std::size_t>(vs1.size()); ++i) TTS_EQUAL(vs1[i], ref[i]);

  auto vs2 = v[sl2];
  for (std::size_t i = 2; i < static_cast<std::size_t>(vs2.size()); ++i) TTS_EQUAL(vs2[i], ref[i]);

  auto vs3 = v[sl3];
  for (std::size_t i = 2; i < static_cast<std::size_t>(vs3.size()); ++i) TTS_EQUAL(vs3[i], ref[i]);

  auto vs4 = v[sl4];
  for (auto i = sl4.begin(); i < sl4.end(); i += sl4.step()) TTS_EQUAL(vs4[(i - sl4.begin()) / sl4.step()], ref[i]);

  auto vs5 = v[sl5];
  for (std::size_t i = 0; i < static_cast<std::size_t>(vs5.size()); ++i) TTS_EQUAL(vs5[i], ref[i]);

  TTS_TYPE_IS(decltype(vs1)::source_type, decltype(v)::source_type);
  TTS_TYPE_IS(decltype(vs2)::source_type, decltype(v)::source_type);
  TTS_TYPE_IS(decltype(vs3)::source_type, decltype(v)::source_type);
  TTS_TYPE_IS(decltype(vs4)::source_type, decltype(v)::source_type);
  TTS_TYPE_IS(decltype(vs5)::source_type, decltype(v)::source_type);
};

TTS_CASE("Indexing Behavior with Boundary Conditions"){
  TTS_WHEN(" - Initialize containers with boundary conditions"){using kwk::_;
using namespace kwk::literals;

float ref[7] = {1, 2, 3, 4, 5, 6, 7};

std::ptrdiff_t N = std::ptrdiff_t(7);

auto v_clamp = kwk::view(kwk::source = ref, kwk::shape{N}, kwk::clamp);
auto v_tiled = kwk::view(kwk::source = ref, kwk::shape{N}, kwk::tile);
auto v_mirro = kwk::view(kwk::source = ref, kwk::shape{N}, kwk::mirror);

TTS_AND_THEN("- Check clamping")
{
  for (std::ptrdiff_t i = 0; i < N; ++i) TTS_EQUAL(v_clamp[i], ref[i]);

  for (std::ptrdiff_t i = N; i < 2 * N; ++i) TTS_EQUAL(v_clamp[i], ref[N]);

  for (std::ptrdiff_t i = -N; i < 0; ++i) TTS_EQUAL(v_clamp[i], ref[0]);
}

TTS_AND_THEN("- Check tiling (Periodicity)")
{
  for (std::ptrdiff_t i = 0; i < N; ++i) TTS_EQUAL(v_tiled[i], ref[i]);

  for (std::ptrdiff_t i = N; i < 2 * N; ++i) TTS_EQUAL(v_tiled[i], ref[i - N]);

  for (std::ptrdiff_t i = -1; i >= -N; --i) TTS_EQUAL(v_tiled[i], ref[N + i]);
}

TTS_AND_THEN("- Check mirroring")
{
  auto expected_mirror_idx = [](std::ptrdiff_t i, std::ptrdiff_t size) {
    if (size <= 1) return std::ptrdiff_t{0};
    std::ptrdiff_t period = 2 * (size - 1);
    std::ptrdiff_t x = i % period;
    if (x < 0) x += period;
    return (x < size) ? x : (period - x);
  };

  for (std::ptrdiff_t i = 0; i < N; ++i) TTS_EQUAL(v_mirro[i], ref[expected_mirror_idx(i, N)]);

  for (std::ptrdiff_t i = N; i < 2 * N; ++i) TTS_EQUAL(v_mirro[i], ref[expected_mirror_idx(i, N)]);

  for (std::ptrdiff_t i = -1; i >= -N; --i) TTS_EQUAL(v_mirro[i], ref[expected_mirror_idx(i, N)]);
}
}
}
;

TTS_CASE("Indexing Behavior with tiles: 1D grid of 2D tiles")
{
  using namespace kwk::literals;

  float ref[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  auto v = kwk::view(kwk::source = ref, kwk::shape{3, kwk::shape{2, 2}});

  for (std::ptrdiff_t i = 0; i < 3; ++i)
  {
    auto curr_tile = v[i];
    TTS_TYPE_IS(decltype(curr_tile)::source_type, decltype(v)::source_type);

    for (std::ptrdiff_t ti = 0; ti < 2; ++ti)
    {
      for (std::ptrdiff_t tj = 0; tj < 2; ++tj) { TTS_EQUAL((curr_tile[ti, tj]), (ref[i * 4 + ti * 2 + tj])); }
    }
  }
};

TTS_CASE("Indexing Behavior with tiles: 2D grid of 1D tiles")
{
  using namespace kwk::literals;

  float ref[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};

  auto v = kwk::view(kwk::source = ref, kwk::shape{kwk::shape{2, 2}, 3});

  for (std::ptrdiff_t i = 0; i < 2; ++i)
  {
    for (std::ptrdiff_t j = 0; j < 2; ++j)
    {
      auto curr_tile = v[i, j];
      TTS_TYPE_IS(decltype(curr_tile)::source_type, decltype(v)::source_type);

      for (std::ptrdiff_t ti = 0; ti < 3; ++ti) { TTS_EQUAL(curr_tile[ti], (ref[i * 6 + j * 3 + ti])); }
    }
  }
};

TTS_CASE("Indexing Behavior with tiles: 2D grid of 2D tiles")
{
  using namespace kwk::literals;

  float ref[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  auto v = kwk::view(kwk::source = ref, kwk::shape{kwk::shape{2, 2}, kwk::shape{2, 2}});

  for (std::ptrdiff_t i = 0; i < 2; ++i)
  {
    for (std::ptrdiff_t j = 0; j < 2; ++j)
    {
      auto curr_tile = v[i, j];
      TTS_TYPE_IS(decltype(curr_tile)::source_type, decltype(v)::source_type);

      for (std::ptrdiff_t ti = 0; ti < 2; ++ti)
      {
        for (std::ptrdiff_t tj = 0; tj < 2; ++tj)
        {
          TTS_EQUAL((curr_tile[ti, tj]), (ref[i * 8 + j * 4 + ti * 2 + tj]));
        }
      }
    }
  }
};

TTS_CASE("Indexing Behavior with tiles: 3D grid with 2D tiles")
{
  using namespace kwk::literals;

  float ref[16] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};

  auto v = kwk::view(kwk::source = ref, kwk::shape{2, 2, kwk::shape{2, 2}});

  for (std::ptrdiff_t i = 0; i < 2; ++i)
  {
    auto sub_range = v[i];
    for (std::ptrdiff_t j = 0; j < 2; ++j)
    {
      auto curr_tile = sub_range[j];
      TTS_TYPE_IS(decltype(curr_tile)::source_type, decltype(v)::source_type);

      for (std::ptrdiff_t ti = 0; ti < 2; ++ti)
      {
        for (std::ptrdiff_t tj = 0; tj < 2; ++tj)
        {
          TTS_EQUAL((curr_tile[ti, tj]), (ref[i * 8 + j * 4 + ti * 2 + tj]));
        }
      }
    }
  }
};
