//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================
#include <kwk/context/sycl/context.hpp>
#include <kwk/algorithm/algos/for_each.hpp>
#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include "test.hpp"
#include <numeric>
#include <benchmark/benchmark.hpp>

TTS_CASE("Check for kwk::reduce(in) 1D")
{
  using data_type = int;
  const std::size_t d0 = 1024 * 1024 * 2; // 256
  const std::size_t input_size = d0;
  std::vector<data_type> input;
  input.resize(input_size);

  // std::array<data_type, input_size> input;

  for (std::size_t i = 0; i < input_size; ++i) { input[i] = i * 3; }

  std::cout << "STD array ok\n";

  auto view_in  = kwk::view{kwk::source = input.data() , kwk::of_size(d0)};

  data_type res_std;

  auto func = [](auto a, auto b) { return (((a + b) % 50) % 25) % 30; };

  auto fct_std = [&]()
  {
    res_std = std::reduce(input.begin(), input.end(), 0, func);
    return res_std;
  };

  kwk::bench::cbench_t b;

  b.start("/home/data_evo/data_sync/academique/These/kiwaku_2025-02/test/bench/bench_files/bench_text.txt", "My first bench!");
  b.set_iterations(10);
  b.run_function("Standard std::reduce", fct_std);
  b.stop();

};

