
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

TTS_CASE("Build a table with dynamic shape settings from a C array")
{
  using namespace kwk::literals;

  float ref[7] = {1, 2, 3, 4, 5, 6, 7};
  float const cref[7] = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7;
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  // sizeof(ptr) + sizeof(deleter) + dynamic_size
  TTS_EQUAL(sizeof(v), (3 * sizeof(void*)));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  // TTS_EXPECT_NOT( v.shape().is_fully_static                );

  sz = 4;
  auto w = kwk::table(kwk::source = cref, kwk::shape{sz});

  TTS_EQUAL(sizeof(w), 3 * sizeof(void*));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 4);
  TTS_EQUAL(w.shape(), kwk::shape{sz});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));

  // TTS_EXPECT_NOT( w.shape().is_fully_static                   );
};

TTS_CASE("Build a 1D table with dynamic shape settings from std::array")
{
  using namespace kwk::literals;

  std::array<float, 7> ref = {1, 2, 3, 4, 5, 6, 7};
  std::array<float, 7> const cref = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7;
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  TTS_EQUAL(sizeof(v), 3 * sizeof(void*));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  for (std::size_t i = 0; i < static_cast<std::size_t>(v.size()); ++i) TTS_EQUAL(v[i], ref[i]);
  TTS_EXPECT_COMPILES(v, { v[0] = 3.f; });

  // TTS_EXPECT_NOT( v.shape().is_fully_static                   );

  sz = 4;
  auto w = kwk::table(kwk::source = cref, kwk::shape{sz});

  TTS_EQUAL(sizeof(w), 3 * sizeof(void*));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 4);
  TTS_EQUAL(w.shape(), kwk::shape{sz});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));

  for (std::size_t i = 0; i < static_cast<std::size_t>(w.size()); ++i) TTS_EQUAL(w[i], cref[i]);
  TTS_EXPECT_NOT_COMPILES(w, { w[0] = 3.f; });
  // TTS_EXPECT_NOT( w.shape().is_fully_static                   );
};

TTS_CASE("Build a 1D table with dynamic shape settings from a ContiguousRange")
{
  using namespace kwk::literals;

  std::vector<float> ref = {1, 2, 3, 4, 5, 6, 7};
  std::vector<float> const cref = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7;
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  TTS_EQUAL(sizeof(v), 3 * sizeof(void*));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  for (std::size_t i = 0; i < static_cast<std::size_t>(v.size()); ++i) TTS_EQUAL(v[i], ref[i]);
  TTS_EXPECT_COMPILES(v, { v[0] = 3.f; });
  // TTS_EXPECT_NOT( v.shape().is_fully_static                );

  sz = 5;
  auto w = kwk::table(kwk::source = cref, kwk::shape{sz});

  TTS_EQUAL(sizeof(w), 3 * sizeof(void*));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 5);
  TTS_EQUAL(w.shape(), kwk::shape{sz});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));

  for (std::size_t i = 0; i < static_cast<std::size_t>(w.size()); ++i) TTS_EQUAL(w[i], cref[i]);
  TTS_EXPECT_NOT_COMPILES(w, { w[0] = 3.f; });
  // TTS_EXPECT_NOT( w.shape().is_fully_static                );
};

TTS_CASE("Build a 1D table with dynamic shape settings from a pointer")
{
  using namespace kwk::literals;

  std::vector<float> ref = {1, 2, 3, 4, 5, 6, 7};
  std::vector<float> const cref = {1, 2, 3, 4, 5, 6, 7};

  auto sz = 7;
  auto v = kwk::table(kwk::source = ref.data(), kwk::shape{sz});

  TTS_EQUAL(sizeof(v), 3 * sizeof(void*));
  TTS_EQUAL(v.ndim, 1UL);
  TTS_EQUAL(v.size(), 7);
  TTS_EQUAL(v.shape(), kwk::shape{sz});
  TTS_EQUAL(v.kind, (kwk::as<float>()));

  for (std::size_t i = 0; i < static_cast<std::size_t>(v.size()); ++i) TTS_EQUAL(v[i], ref[i]);
  TTS_EXPECT_COMPILES(v, { v[0] = 3.f; });
  // TTS_EXPECT_NOT( v.shape().is_fully_static                );

  sz = 5;
  auto w = kwk::table(kwk::source = cref.data(), kwk::shape{sz});

  TTS_EQUAL(sizeof(w), 3 * sizeof(void*));
  TTS_EQUAL(w.ndim, 1UL);
  TTS_EQUAL(w.size(), 5);
  TTS_EQUAL(w.shape(), kwk::shape{sz});
  TTS_EQUAL(w.kind, (kwk::as<float const>()));

  for (std::size_t i = 0; i < static_cast<std::size_t>(w.size()); ++i) TTS_EQUAL(w[i], cref[i]);
  TTS_EXPECT_NOT_COMPILES(w, { w[0] = 3.f; });
  // TTS_EXPECT_NOT( w.shape().is_fully_static                );
};
