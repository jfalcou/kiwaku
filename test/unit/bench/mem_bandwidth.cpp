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

// template<typename DATA_TYPE>
// void test_find(auto&& policy, std::size_t const size)
// {
//   std::size_t count = 0;
//   auto f = [&count](DATA_TYPE const& e) { if (e == 2) count = e; };

//   std::size_t elements = size / sizeof(DATA_TYPE);
  
//   std::vector<DATA_TYPE> data;
//   data.resize(elements);


//   std::fill(data.begin(), data.end(), 1);
//   data[elements - 1] = 2;
//   ::sutils::chrono_t chrono;
//   chrono.Init();
//   std::for_each(policy, data.begin(), data.end(), f);
//   double elapsed = chrono.ElapsedTime();

//   TTS_EQUAL(count, elements);

//   double bandwidth = size / elapsed; // read only
//   double bandwidthGB = bandwidth / (1000000000.);

//   std::string policy_str = "unknown_policy";
//   if (typeid(policy) == typeid(std::execution::unseq))     policy_str = "unseq";
//   if (typeid(policy) == typeid(std::execution::seq))       policy_str = "seq";
//   if (typeid(policy) == typeid(std::execution::par))       policy_str = "par";
//   if (typeid(policy) == typeid(std::execution::par_unseq)) policy_str = "par_unseq";

//   std::cout << "Memory bandwidth for " << policy_str << " GB/s: " << bandwidthGB << "  -  elapsed " << std::floor(elapsed * 1000) << " ms." << "\n\n";
// }

template<typename DATA_TYPE>
void test_read(auto&& policy, std::size_t const size)
{
  std::size_t count = 0;
  auto f = [&count](DATA_TYPE const& e) { count += e; };

  std::size_t elements = size / sizeof(DATA_TYPE);
  
  std::vector<DATA_TYPE> data;
  data.resize(elements);


  std::fill(data.begin(), data.end(), 1);
  ::sutils::chrono_t chrono;
  chrono.Init();
  std::for_each(policy, data.begin(), data.end(), f);
  double elapsed = chrono.ElapsedTime();

  TTS_EQUAL(count, elements);

  double bandwidth = size / elapsed; // read only
  double bandwidthGB = bandwidth / (1000000000.);

  std::string policy_str = "unknown_policy";
  if (typeid(policy) == typeid(std::execution::unseq))     policy_str = "unseq";
  if (typeid(policy) == typeid(std::execution::seq))       policy_str = "seq";
  if (typeid(policy) == typeid(std::execution::par))       policy_str = "par";
  if (typeid(policy) == typeid(std::execution::par_unseq)) policy_str = "par_unseq";

  std::cout << "Memory bandwidth for " << policy_str << " GB/s: " << bandwidthGB << "  -  elapsed " << std::floor(elapsed * 1000) << " ms." << "\n\n";
}

template<typename DATA_TYPE>
void test1(auto&& policy, std::size_t const size)
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

  double bandwidth = (elements * 2 * sizeof(DATA_TYPE)) / elapsed; // read + write
  double bandwidthGB = bandwidth / (1000000000.);

  std::string policy_str = "unknown_policy";
  if (typeid(policy) == typeid(std::execution::unseq))     policy_str = "unseq";
  if (typeid(policy) == typeid(std::execution::seq))       policy_str = "seq";
  if (typeid(policy) == typeid(std::execution::par))       policy_str = "par";
  if (typeid(policy) == typeid(std::execution::par_unseq)) policy_str = "par_unseq";

  std::cout << "Memory bandwidth for " << policy_str << " GB/s: " << bandwidthGB << "  -  elapsed " << std::floor(elapsed * 1000) << " ms." << "\n\n";
}


template<typename DATA_TYPE>
void test2(auto&& policy, std::size_t const size)
{

  std::size_t elements = (size / sizeof(DATA_TYPE)) / 2;
  
  std::vector<DATA_TYPE> data_in;
  std::vector<DATA_TYPE> data_out;
  data_in.resize(elements);
  data_out.resize(elements);

  auto f = [&](DATA_TYPE const& in)
  {
    std::size_t index = &in - &data_in[0];
    data_out[index] = in + 1;
  };

  std::fill(data_out.begin(), data_out.end(), 0);
  std::fill(data_in.begin() , data_in.end(), 1);

  ::sutils::chrono_t chrono;
  chrono.Init();
  std::for_each(policy, data_in.begin(), data_in.end(), f);
  double elapsed = chrono.ElapsedTime();

  std::size_t sum = 0;
  for (DATA_TYPE const& e : data_out) sum += e;

  TTS_EQUAL(sum, elements * 2);

  double bandwidth = (elements * 3 * sizeof(DATA_TYPE)) / elapsed; // read + (read + write)
  double bandwidthGB = bandwidth / (1000000000.);

  std::string policy_str = "unknown_policy";
  if (typeid(policy) == typeid(std::execution::unseq))     policy_str = "unseq";
  if (typeid(policy) == typeid(std::execution::seq))       policy_str = "seq";
  if (typeid(policy) == typeid(std::execution::par))       policy_str = "par";
  if (typeid(policy) == typeid(std::execution::par_unseq)) policy_str = "par_unseq";

  std::cout << "Memory bandwidth 2 for " << policy_str << " GB/s: " << bandwidthGB << "  -  elapsed " << std::floor(elapsed * 1000) << " ms." << "\n\n";
}


TTS_CASE("Benchmark - for_each TEST MEMORY BANDWIDTH")
{
  if (::kwk::bench::enable_global)
  {
    using DATA_TYPE = unsigned int;

    sutils::printer_t::head("TEST 1");
    test1<DATA_TYPE>(std::execution::seq       , 4000000000UL * 1UL);
    test1<DATA_TYPE>(std::execution::unseq     , 4000000000UL * 1UL);
    test1<DATA_TYPE>(std::execution::par       , 4000000000UL * 1UL);
    test1<DATA_TYPE>(std::execution::par_unseq , 4000000000UL * 1UL);

    sutils::printer_t::head("TEST 2");
    test2<DATA_TYPE>(std::execution::seq       , 4000000000UL * 1UL);
    test2<DATA_TYPE>(std::execution::unseq     , 4000000000UL * 1UL);
    test2<DATA_TYPE>(std::execution::par       , 4000000000UL * 1UL);
    test2<DATA_TYPE>(std::execution::par_unseq , 4000000000UL * 1UL);
  }
  else
  {
    TTS_EQUAL(true, true);
  }
};

// LEGEND

// [sylvainj@parsys-legend kiwaku_build_2025-06]$ ninja unit.bench.mem_bandwidth.exe && ./unit/unit.bench.mem_bandwidth.exe
// [0/2] Re-checking globbed directories...
// [1/2] Building CXX object test/CMakeFiles/unit.bench.mem_bandwidth.exe.dir/unit/bench/mem_bandwidth.cpp.o
// icpx: warning: -ltbb: 'linker' input unused [-Wunused-command-line-argument]
// icpx: warning: -ltbb: 'linker' input unused [-Wunused-command-line-argument]
// [2/2] Linking CXX executable unit/unit.bench.mem_bandwidth.exe
// [Kiwaku] - Assertions: Disabled
// ===================================================
// ===================================================
// ====================- TEST 1 -=====================
// ===================================================
// ===================================================

// Memory bandwidth for seq GB/s: 41.4287  -  elapsed 193 ms.

// Memory bandwidth for unseq GB/s: 41.41  -  elapsed 193 ms.

// Memory bandwidth for par GB/s: 36.3593  -  elapsed 220 ms.

// Memory bandwidth for par_unseq GB/s: 36.5435  -  elapsed 218 ms.

// ===================================================
// ===================================================
// ====================- TEST 2 -=====================
// ===================================================
// ===================================================

// Memory bandwidth 2 for seq GB/s: 40.0515  -  elapsed 149 ms.

// Memory bandwidth 2 for unseq GB/s: 40.0622  -  elapsed 149 ms.

// Memory bandwidth 2 for par GB/s: 36.9677  -  elapsed 162 ms.

// Memory bandwidth 2 for par_unseq GB/s: 36.9321  -  elapsed 162 ms.

// [V] - Benchmark - for_each TEST MEMORY BANDWIDTH
// ----------------------------------------------------------------
// Results: 8 tests - 8/8 successes - 0/0 failure - 0/0 invalid
// [sylvainj@parsys-legend kiwaku_build_2025-06]$ ninja unit.bench.mem_bandwidth.exe && ./unit/unit.bench.mem_bandwidth.exe
// [0/2] Re-checking globbed directories...
// ninja: no work to do.
// [Kiwaku] - Assertions: Disabled
// ===================================================
// ===================================================
// ====================- TEST 1 -=====================
// ===================================================
// ===================================================

// Memory bandwidth for seq GB/s: 41.4229  -  elapsed 193 ms.

// Memory bandwidth for unseq GB/s: 41.5353  -  elapsed 192 ms.

// Memory bandwidth for par GB/s: 36.415  -  elapsed 219 ms.

// Memory bandwidth for par_unseq GB/s: 36.5323  -  elapsed 218 ms.

// ===================================================
// ===================================================
// ====================- TEST 2 -=====================
// ===================================================
// ===================================================

// Memory bandwidth 2 for seq GB/s: 40.0447  -  elapsed 149 ms.

// Memory bandwidth 2 for unseq GB/s: 39.9843  -  elapsed 150 ms.

// Memory bandwidth 2 for par GB/s: 36.9613  -  elapsed 162 ms.

// Memory bandwidth 2 for par_unseq GB/s: 36.9538  -  elapsed 162 ms.

// [V] - Benchmark - for_each TEST MEMORY BANDWIDTH
// ----------------------------------------------------------------
// Results: 8 tests - 8/8 successes - 0/0 failure - 0/0 invalid
// [sylvainj@parsys-legend kiwaku_build_2025-06]$ 


// AVX512
// [sylvainj@falcou-avx512 kiwaku_build]$ ninja unit.bench.mem_bandwidth.exe && ./unit/unit.bench.mem_bandwidth.exe
// [0/2] Re-checking globbed directories...
// [1/3] Building CXX object test/CMakeFiles/unit.bench.mem_bandwidth.exe.dir/unit/bench/mem_bandwidth.cpp.o
// icpx: warning: -ltbb: 'linker' input unused [-Wunused-command-line-argument]
// icpx: warning: -ltbb: 'linker' input unused [-Wunused-command-line-argument]
// [2/3] Linking CXX executable unit/unit.bench.mem_bandwidth.exe
// [Kiwaku] - Assertions: Disabled

// ====================- TEST 1 -=====================
// Memory bandwidth for seq GB/s: 33.2408  -  elapsed 240 ms.
// Memory bandwidth for unseq GB/s: 33.2415  -  elapsed 240 ms.
// Memory bandwidth for par GB/s: 35.4922  -  elapsed 225 ms.
// Memory bandwidth for par_unseq GB/s: 35.5511  -  elapsed 225 ms.

// ====================- TEST 2 -=====================
// Memory bandwidth 2 for seq GB/s: 32.6223  -  elapsed 183 ms.
// Memory bandwidth 2 for unseq GB/s: 32.6074  -  elapsed 184 ms.
// Memory bandwidth 2 for par GB/s: 34.9575  -  elapsed 171 ms.
// Memory bandwidth 2 for par_unseq GB/s: 34.9361  -  elapsed 171 ms.

// [V] - Benchmark - for_each TEST MEMORY BANDWIDTH
// ----------------------------------------------------------------
// Results: 8 tests - 8/8 successes - 0/0 failure - 0/0 invalid
// [sylvainj@falcou-avx512 kiwaku_build]$ ninja unit.bench.mem_bandwidth.exe && ./unit/unit.bench.mem_bandwidth.exe
// [0/2] Re-checking globbed directories...
// [1/2] Linking CXX executable unit/unit.bench.mem_bandwidth.exe
// [Kiwaku] - Assertions: Disabled

// ====================- TEST 1 -=====================
// Memory bandwidth for seq GB/s: 33.2488  -  elapsed 240 ms.
// Memory bandwidth for unseq GB/s: 33.218  -  elapsed 240 ms.
// Memory bandwidth for par GB/s: 35.5201  -  elapsed 225 ms.
// Memory bandwidth for par_unseq GB/s: 35.5415  -  elapsed 225 ms.

// ====================- TEST 2 -=====================
// Memory bandwidth 2 for seq GB/s: 32.63  -  elapsed 183 ms.
// Memory bandwidth 2 for unseq GB/s: 32.6469  -  elapsed 183 ms.
// Memory bandwidth 2 for par GB/s: 34.9518  -  elapsed 171 ms.
// Memory bandwidth 2 for par_unseq GB/s: 34.9494  -  elapsed 171 ms.

// [V] - Benchmark - for_each TEST MEMORY BANDWIDTH
// ----------------------------------------------------------------
// Results: 8 tests - 8/8 successes - 0/0 failure - 0/0 invalid
// [sylvainj@falcou-avx512 kiwaku_build]$ 