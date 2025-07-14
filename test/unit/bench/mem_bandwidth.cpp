//==================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//==================================================================================================

#include "include/benchmark.hpp"
#include "include/utils/utils.hpp"


#include <cstdlib>
#include "test.hpp"
#include <numeric>
#include <optional>
#include <cmath>
#include <execution> // don't forget the -ltbb compiler flag


template<typename DATA_TYPE>
void test(auto&& policy, std::size_t const size)
{
  auto f = [](DATA_TYPE& e) { e += 1; };

  std::size_t elements = size / sizeof(DATA_TYPE);
  
  std::vector<DATA_TYPE> data;
  data.resize(elements);


  std::fill(data.begin(), data.end(), 0);
  ::sutils::chrono_t chrono;
  chrono.Init();
  std::for_each(policy, data.begin(), data.end(), f);
  double elapsed = chrono.ElapsedTime();

  std::size_t sum = 0;
  for (DATA_TYPE const& e : data) sum += e;

  TTS_EQUAL(sum, elements);

  double bandwidth = size / elapsed;
  double bandwidthGB = bandwidth / (1000000000.);

  std::string policy_str = "unknown_policy";
  if (typeid(policy) == typeid(std::execution::unseq))     policy_str = "unseq";
  if (typeid(policy) == typeid(std::execution::seq))       policy_str = "seq";
  if (typeid(policy) == typeid(std::execution::par))       policy_str = "par";
  if (typeid(policy) == typeid(std::execution::par_unseq)) policy_str = "par_unseq";


  // switch(typeid(policy))
  // {
  //   case typeid(std::execution::unseq): policy_str = "unseq"; break;
  //   case typeid(std::execution::seq): policy_str = "seq"; break;
  //   case typeid(std::execution::par): policy_str = "par"; break;
  //   case typeid(std::execution::par_unseq): policy_str = "par_unseq"; break;
  //   default: break;
  // }

  std::cout << "Memory bandwidth for " << policy_str << " GB/s: " << bandwidthGB << "  -  elapsed " << std::floor(elapsed * 1000) << " ms." << "\n\n";
}


TTS_CASE("Benchmark - for_each TEST MEMORY BANDWIDTH")
{
  if (::kwk::bench::enable_global)
  {
    using DATA_TYPE = unsigned int;

    test<DATA_TYPE>(std::execution::seq       , 4000000000UL * 1UL);
    test<DATA_TYPE>(std::execution::unseq     , 4000000000UL * 1UL);
    test<DATA_TYPE>(std::execution::par       , 4000000000UL * 1UL);
    test<DATA_TYPE>(std::execution::par_unseq , 4000000000UL * 1UL);
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};


