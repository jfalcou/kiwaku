//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/detail.hpp>
#include <kwk/concepts.hpp>
#include <kwk/utility.hpp>
#include <kwk/settings/options.hpp>
#include <kwk/settings/source.hpp>

#include <vector>
#include <array>
#include <list>
#include <span>

using container_types = tts::types<std::vector<float>, std::array<int, 10>, std::span<short>, long[10]>;

TTS_CASE_TPL("Check if source behave as expected in options", container_types)<typename T>(tts::type<T>)
{
  auto o = kwk::options(kwk::source = T{});
  TTS_EXPECT_COMPILES(o, { o[kwk::source]; });
};

TTS_CASE("Check if special source case behave as expected")
{
  auto r = std::list<int>{};
  TTS_EXPECT_NOT_COMPILES(r, { kwk::options(kwk::source = r); });

  auto x = std::array<int, 10>{};
  auto sp = std::span<int, 5>{x.data(), 5};
  TTS_EXPECT_COMPILES(sp, { kwk::options(kwk::source = sp); });
};
