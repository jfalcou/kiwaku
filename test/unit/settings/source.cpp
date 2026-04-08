//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include "test.hpp"
// #include <kwk/concepts.hpp>
// #include <kwk/utility.hpp>
// #include <kwk/settings.hpp>
#include <kwk/kwk.hpp>

#include <vector>
#include <array>
#include <list>
#include <span>

// using container_types = tts::types<std::vector<float>, std::array<int, 10>, std::span<short>>; //, long[10]>;
//
// TTS_CASE_TPL("Check if source behave as expected in options", container_types)<typename T>(tts::type<T>)
//{
//   auto o = kwk::options(kwk::source = T{});
//   TTS_EXPECT_COMPILES(o, { o[kwk::source]; });
// };

TTS_CASE("Check if special source behave as expected")
{
  using namespace kwk::literals;
  auto r = std::list<int>{};
  TTS_EXPECT_NOT_COMPILES(r, { kwk::options(kwk::source = r); });

  auto a = std::array<int, 10>{};
  auto f_sp = std::span<int, 5>{a.data(), 5};
  auto v = std::vector<float>{};
  auto sv = std::vector<int>{3, 4, 5, 6};
  auto d_sp = std::span<float>{v};
  long c_array[10] = {};
  double* ptr;

  {
    auto opt = kwk::options(kwk::source = a);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, int);
    TTS_EQUAL(storage(opt[kwk::source]), a.data());
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{10_c});
  }
  {
    auto opt = kwk::options(kwk::source = f_sp);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, int);
    TTS_EQUAL(storage(opt[kwk::source]), f_sp.data());
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{5_c});
  }
  {
    auto opt = kwk::options(kwk::source = v);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, float);
    TTS_EQUAL(storage(opt[kwk::source]), v.data());
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{kwk::_});
  }
  {
    auto opt = kwk::options(kwk::source = sv);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, int);
    TTS_EQUAL(storage(opt[kwk::source]), sv.data());
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{4});
  }
  {
    auto opt = kwk::options(kwk::source = d_sp);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, float);
    TTS_EQUAL(storage(opt[kwk::source]), d_sp.data());
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{kwk::_});
  }
  {
    auto opt = kwk::options(kwk::source = c_array);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, long);
    TTS_EQUAL(storage(opt[kwk::source]), c_array);
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{10_c});
  }
  {
    auto opt = kwk::options(kwk::source = ptr);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, double);
    TTS_EQUAL(storage(opt[kwk::source]), ptr);
    TTS_EQUAL(shape_of(opt[kwk::source]), kwk::shape{kwk::_});
  }
};

TTS_CASE("Check if source option behave as expected with const qualified containers")
{
  auto const r = std::list<int>{};
  TTS_EXPECT_NOT_COMPILES(r, { kwk::options(kwk::source = r); });

  auto base_a = std::array<int, 10>{};
  auto base_v = std::vector<float>{};

  auto const a = std::array<int, 10>{};
  auto const f_sp = std::span<int, 5>{base_a.data(), 5};
  auto const v = std::vector<float>{};
  auto const d_sp = std::span<float>{base_v};
  long const c_array[10] = {};
  double const* ptr;

  {
    auto opt = kwk::options(kwk::source = a);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, int const);
    TTS_EQUAL(storage(opt[kwk::source]), a.data());
  }
  {
    auto opt = kwk::options(kwk::source = f_sp);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, int const);
    TTS_EQUAL(storage(opt[kwk::source]), f_sp.data());
  }
  {
    auto opt = kwk::options(kwk::source = v);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, float const);
    TTS_EQUAL(storage(opt[kwk::source]), v.data());
  }
  {
    auto opt = kwk::options(kwk::source = d_sp);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, float const);
    TTS_EQUAL(storage(opt[kwk::source]), d_sp.data());
  }
  {
    auto opt = kwk::options(kwk::source = c_array);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, long const);
    TTS_EQUAL(storage(opt[kwk::source]), c_array);
  }
  {
    auto opt = kwk::options(kwk::source = ptr);
    TTS_TYPE_IS(decltype(opt[kwk::source])::element_type, double const);
    TTS_EQUAL(storage(opt[kwk::source]), ptr);
  }
};
