//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================
// #include "../generic/replace.hpp"
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"

#include <kwk/context/sycl/context.hpp>
#include <kwk/context/cpu/context.hpp>

TTS_CASE("Check for kwk::reduce(in) 1D")
{
  const std::size_t data_size = 10;

  int data[data_size];

  for (std::size_t i = 0; i < data_size; ++i) { data[i] = 2; }

  auto kv = kwk::view{kwk::source = data, kwk::of_size(data_size)};

  auto res = kwk::reduce(kwk::sycl::default_context, kv);

  TTS_EQUAL(res, static_cast<int>(data_size) * 2);
};
