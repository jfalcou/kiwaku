//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
#include <kwk/concepts.hpp>
#include <kwk/utility.hpp>
#include <kwk/settings.hpp>

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

  auto a = std::array<int, 10>{};
  auto f_sp = std::span<int, 5>{a.data(), 5};
  auto v = std::vector<float>{};
  auto d_sp = std::span<float>{v};
  long c_array[10] = {};
  double* ptr;

  {
    auto opt = kwk::options(kwk::source = a);
    TTS_TYPE_IS(std::remove_cvref_t<decltype(opt[kwk::source])>, (kwk::__::array_option<int, 10>));
    TTS_EQUAL(storage(opt[kwk::source]), a.data());
  }
  {
    auto opt = kwk::options(kwk::source = f_sp);
    TTS_TYPE_IS(decltype(opt[kwk::source]), (kwk::__::array_option<int, 5>));
    TTS_EQUAL(storage(opt[kwk::source]), f_sp.data());
  }
  {
    auto opt = kwk::options(kwk::source = v);
    TTS_TYPE_IS(decltype(opt[kwk::source]), (kwk::__::range_option<float>));
    TTS_EQUAL(storage(opt[kwk::source]), v.data());
  }
  {
    auto opt = kwk::options(kwk::source = d_sp);
    TTS_TYPE_IS(decltype(opt[kwk::source]), (kwk::__::range_option<float>));
    TTS_EQUAL(storage(opt[kwk::source]), d_sp.data());
  }
  {
    auto opt = kwk::options(kwk::source = c_array);
    TTS_TYPE_IS(decltype(opt[kwk::source]), (kwk::__::array_option<long, 10>));
    TTS_EQUAL(storage(opt[kwk::source]), &c_array[0]);
  }
  {
    auto opt = kwk::options(kwk::source = ptr);
    TTS_TYPE_IS(decltype(opt[kwk::source]), (kwk::__::pointer_option<double>));
    TTS_EQUAL(storage(opt[kwk::source]), ptr);
  }
};
