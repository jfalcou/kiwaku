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
  auto v = kwk::table(kwk::source = ref, kwk::shape{sz});

  for (std::size_t i = 0; i < static_cast<std::size_t>(v.size()); ++i) TTS_EQUAL(v[i], ref[i]);
  TTS_EXPECT_COMPILES(v, { v[0] = 3.f; });

  auto w = kwk::table(kwk::source = cref, kwk::shape{sz});
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

  TTS_TYPE_IS(decltype(vs1)::source_type, decltype(v)::source_type, );
  TTS_TYPE_IS(decltype(vs2)::source_type, decltype(v)::source_type, );
  TTS_TYPE_IS(decltype(vs3)::source_type, decltype(v)::source_type, );
  TTS_TYPE_IS(decltype(vs4)::source_type, decltype(v)::source_type, );
  TTS_TYPE_IS(decltype(vs5)::source_type, decltype(v)::source_type, );
};
