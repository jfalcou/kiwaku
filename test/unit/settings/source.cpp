//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/settings/options.hpp>
#include <kwk/settings/source.hpp>

#include <vector>
#include <array>
#include <list>
#include <span>

using container_types = tts_types<std::vector<float>,
                                  std::array<int, 10>,
                                  std::span<short>,
                                  std::list<double>,
                                  long[10],
                                  std::span<long long, 10>>

  TTS_CASE_TPL("kind behave as expected in options", container_types)<typename T>(tts::type<T>)
{
  auto o = kwk::options(kwk::source = T{});
  TTS_TYPE_IS(decltype(o[kwk::source]), T);
};
