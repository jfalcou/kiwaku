//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
#include <cstdlib>
#include <kwk/algorithm/algos/copy.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <kwk/context/cpu/context.hpp>

// copy 1D

TTS_CASE("Check for kwk::copy(kwk::cpu, out, in) 1D")
{
  const std::size_t input_size = 200;
  const std::size_t output_size = input_size;
  std::array<int, input_size> input;
  std::array<int, output_size> output;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; };

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(output_size)};
  
  kwk::copy(kwk::cpu, view_output, view_input);

  TTS_ALL_EQUAL(input, output);
};

TTS_CASE("Check for kwk::copy(kwk::cpu, out, in) 1D with double")
{
  const std::size_t input_size = 200;
  const std::size_t output_size = input_size;
  std::array<double, input_size> input;
  std::array<double, output_size> output;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; };

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(output_size)};
  
  kwk::copy(kwk::cpu, view_output, view_input);

  TTS_ALL_EQUAL(input, output);
};

TTS_CASE("Check for kwk::copy(kwk::cpu, out, in) 1D size 1000000")
{
  const std::size_t input_size = 1000000;
  const std::size_t output_size = input_size;
  std::array<int, input_size> input;
  std::array<int, output_size> output;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; };

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(output_size)};
  
  kwk::copy(kwk::cpu, view_output, view_input);

  TTS_ALL_EQUAL(input, output);
};

TTS_CASE("Check for kwk::copy(kwk::cpu, out, in) 1D size 1")
{
  const std::size_t input_size = 1;
  const std::size_t output_size = input_size;
  std::array<int, input_size> input;
  std::array<int, output_size> output;
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; };

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(output_size)};
  
  kwk::copy(kwk::cpu, view_output, view_input);

  TTS_ALL_EQUAL(input, output);
};

TTS_CASE("Check for kwk::copy(kwk::cpu, out, in) 1D size 0")
{
  std::vector<int> input;
  std::vector<int> output;

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(0)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(0)};
  
  kwk::copy(kwk::cpu, view_output, view_input);

  TTS_EQUAL(output.size(), 0UL);
};


TTS_CASE("Check for kwk::copy_if(kwk::cpu, func, out, in) 1D")
{
  const std::size_t input_size = 200;
  const std::size_t output_size = input_size;
  std::array<double, input_size> input;
  std::array<double, output_size> output;
  std::array<double, output_size> expected;
  input.fill(0);
  output.fill(0);
  expected.fill(0);

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(input_size)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(output_size)};

  std::size_t count = 0; // Only compatible with sequential CPU code...
  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i; };
  auto func = [&](auto e) { ++count; return (e < 101); };

  for (std::size_t i = 0; i < output_size; ++i)
  { 
    expected[i] = func(input[i]) ? input[i] : 0;
  };
  TTS_EQUAL(count, output_size);

  count = 0;
  kwk::copy_if(kwk::cpu, func, view_output, view_input);
  TTS_EQUAL(count, output_size);

  TTS_ALL_EQUAL(output, expected);
};



TTS_CASE("Check for kwk::copy(kwk::cpu, out, in, func) 1D size 0")
{
  std::vector<int> input;
  std::vector<int> output;

  auto view_input = kwk::view{kwk::source = input, kwk::of_size(0)};
  auto view_output = kwk::view{kwk::source = output, kwk::of_size(0)};
  auto func = [&](auto e) { return (e < 101); };
  
  kwk::copy_if(kwk::cpu, func, view_output, view_input);

  TTS_EQUAL(output.size(), 0UL);
};