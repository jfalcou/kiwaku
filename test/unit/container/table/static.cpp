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
#include <array>
#include <vector>

TTS_CASE("Build a table with static shape settings from a C array")
{
  using namespace kwk::literals;

  float ref[7] = {1, 2, 3, 4, 5, 6, 7};
  float const cref[7] = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7_c;
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  TTS_EQUAL(sizeof(v), (7 * sizeof(float)));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  auto sz2 = 4_c;
  auto w = kwk::table(kwk::source = cref, kwk::shape{sz2});

  TTS_EQUAL(sizeof(w), (4 * sizeof(float)));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 4);
  TTS_EQUAL(w.shape(), kwk::shape{sz2});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));
};

TTS_CASE("Build a 1D table with static shape settings from std::array")
{
  using namespace kwk::literals;

  std::array<float, 7> ref = {1, 2, 3, 4, 5, 6, 7};
  std::array<float, 7> const cref = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7_c;
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  TTS_EQUAL(sizeof(v), (7 * sizeof(float)));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  auto sz2 = 4_c;
  auto w = kwk::table(kwk::source = cref, kwk::shape{sz2});

  TTS_EQUAL(sizeof(w), (4 * sizeof(float)));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 4);
  TTS_EQUAL(w.shape(), kwk::shape{sz2});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));
};

TTS_CASE("Build a 1D table with static shape settings from a ContiguousRange")
{
  using namespace kwk::literals;

  std::vector<float> ref = {1, 2, 3, 4, 5, 6, 7};
  std::vector<float> const cref = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7_c;
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  TTS_EQUAL(sizeof(v), (7 * sizeof(float)));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  auto sz2 = 5_c;
  auto w = kwk::table(kwk::source = cref, kwk::shape{sz2});

  TTS_EQUAL(sizeof(w), (5 * sizeof(float)));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 5);
  TTS_EQUAL(w.shape(), kwk::shape{sz2});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));
};

TTS_CASE("Build a 1D table with static shape settings from a pointer")
{
  using namespace kwk::literals;

  std::vector<float> ref = {1, 2, 3, 4, 5, 6, 7};
  std::vector<float> const cref = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7_c;
  auto v = kwk::table(kwk::source = ref.data(), kwk::shape{sz});

  TTS_EQUAL(sizeof(v), (7 * sizeof(float)));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  auto sz2 = 5_c;
  auto w = kwk::table(kwk::source = cref.data(), kwk::shape{sz2});

  TTS_EQUAL(sizeof(w), (5 * sizeof(float)));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 5);
  TTS_EQUAL(w.shape(), kwk::shape{sz2});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));
};
