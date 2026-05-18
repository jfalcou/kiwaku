//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/predicates.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

TTS_CASE("Check for kwk::all_of(in, func) 1D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  std::size_t count = 0;
  auto res = kwk::all_of( view_in
                        , [&](auto e) { ++count; return (e % 3 == 0); }
                        );

  TTS_EQUAL(res, true);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::all_of(in, func) 1D false")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }
  input[200] = 8;

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  std::size_t count = 0;
  auto res = kwk::all_of( view_in
                        , [&](auto e) { ++count; return (e % 3 == 0); }
                        );

  TTS_EQUAL(res, false);
  TTS_EQUAL(count, 201UL);
};


TTS_CASE("Check for kwk::any_of(in, func) 1D true")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2 + 1; }
  input[200] = 222;

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  std::size_t count = 0;
  auto res = kwk::any_of( view_in
                        , [&](auto e) { ++count; return (e % 2 == 0); }
                        );

  TTS_EQUAL(res, true);
  TTS_EQUAL(count, 201UL);
};

TTS_CASE("Check for kwk::any_of(in, func) 1D false")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2 + 1; }

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  std::size_t count = 0;
  auto res = kwk::any_of( view_in
                        , [&](auto e) { ++count; return (e % 2 == 0); }
                        );

  TTS_EQUAL(res, false);
  TTS_EQUAL(count, input_size);
};


TTS_CASE("Check for kwk::none_of(in, func) 1D true")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2 + 1; }
  // input[200] = 222;

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  std::size_t count = 0;
  auto res = kwk::none_of( view_in
                        , [&](auto e) { ++count; return (e % 2 == 0); }
                        );

  TTS_EQUAL(res, true);
  TTS_EQUAL(count, input_size);
};

TTS_CASE("Check for kwk::none_of(in, func) 1D false")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2 + 1; }
  input[200] = 222;

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  std::size_t count = 0;
  auto res = kwk::none_of( view_in
                        , [&](auto e) { ++count; return (e % 2 == 0); }
                        );

  TTS_EQUAL(res, false);
  TTS_EQUAL(count, 201UL);
};


TTS_CASE("Check for kwk::count(in, value) 1D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  input[100] = 8;
  input[234] = 8;
  input[311] = 8;
  input[312] = 8;

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  auto res = kwk::count(view_in, 8);

  TTS_EQUAL(res, 5UL);
};

TTS_CASE("Check for kwk::count_if(in, func) 1D")
{
  using data_type = int;
  const std::size_t d0 = 471;
  const std::size_t input_size = d0;
  std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 2; }
  input[234] = 8;
  input[311] = 8;
  input[312] = 8;

  auto view_in  = kwk::view{kwk::source = input , kwk::of_size(d0)};

  auto res = kwk::count_if(view_in, [](auto e) { return e <= 100; });

  TTS_EQUAL(res, 54UL);
};

