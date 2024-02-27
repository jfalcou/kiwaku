//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================

// Exécution GPU SYCL :
// icpx -fsycl -fsycl-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda reduce.cpp -o e -O3 -std=c++20 -I/mnt/chaton/kiwaku/include && ./e

// Exécution CPU SYCL :
// icpx -fsycl reduce.cpp -o e -O3 -std=c++20 -I/mnt/chaton/kiwaku/include && ./e


#include <kwk/algorithm/algos/reduce.hpp>
#include <kwk/container.hpp>
#include <cmath>

#include <kwk/context/sycl/context.hpp>
#include <kwk/context/cpu/context.hpp>
#include "../utils/utils.hpp"

#define HEAVY true
#define INCREASE_SIZE_COUNT 20
#define REPEAT_ITERATION_COUNT 6
// #define ARRAY_LENGTH 102400000UL
#define ARRAY_LENGTH 25600000UL

std::string make_prefix()
{
  return get_computer_name() + "_" + (HEAVY?"heavy":"copy")
         + "_isc" + std::to_string(INCREASE_SIZE_COUNT) 
         + "_rp" + std::to_string(REPEAT_ITERATION_COUNT)
         + "_al" + std::to_string(ARRAY_LENGTH)
         + "_";
}

#if HEAVY
  // auto global_fct = [](auto init, auto x)
  // {
  //   return init + 1 / ((1 - std::atan(x))
  //           + (1
  //             / (2
  //               + std::atan( 5 / (3 + std::atan(std::atan(std::atan(std::atan(x)))))))));
  // };
  auto global_fct = [](auto init, auto x)
  {
    return init + x/2;
  };
#else
  auto global_fct = [](auto init, auto x) { return init + x; }; // 1.0/(1.0+e)
#endif


namespace bench
{
  struct result_item
  {
    result_item(std::size_t witem_count_) : witem_count(witem_count_) {}
    std::size_t witem_count;
    std::vector<std::size_t> host_alloc, copies_and_kernel, check;
    // Nouvelle allocation mémoire hôte à chaque fois pour éviter que le compilo n'optimise les calculs
  };

  struct result_vector
  {
    // Elapsed time in milliseconds
    std::vector<result_item> items;

    void clear() { items.clear(); }

    void write_to_gfile()
    {
      // array size
      // h1 h2 h3 ...
      // c1 c2 c3 ...
      // k1 k2 k3 ...
      // array size
      // ...
      for (result_item& item : items)
      {
        write_f << item.witem_count << "\n";
        for (std::size_t i2 = 0; i2 < item.check.size(); ++i2) {
          write_f << item.host_alloc[i2] << " ";
        }
        write_f << "\n";
        for (std::size_t i2 = 0; i2 < item.check.size(); ++i2) {
          write_f << item.copies_and_kernel[i2] << " ";
        }
        write_f << "\n";
        for (std::size_t i2 = 0; i2 < item.check.size(); ++i2) {
          write_f << item.check[i2] << " ";
        }
        write_f << "\n";
      }
    }
  };
}

void timer(std::vector<std::size_t>& v, bench::chrono_t& c)
{
  v.push_back(c.ElapsedTimeMSReset());
}

void check_tolerance(auto v1, auto v2)
{
  const double ERROR_TOLERANCE = 0.001;
  double diff = std::abs(v1 - v2);
  double moy  = std::abs(v1 + v2) / 2;
  std::cout << "           check_tolerance: diff(" << diff << ") moy(" << moy << ")\n";
  assert(diff < moy * ERROR_TOLERANCE);
}

template<typename Context>
bench::result_item main_bench(Context& ctx, std::size_t witem_count, std::size_t repeat_count)
{
  std::size_t array_length = ARRAY_LENGTH;
  bench::result_item ritem{witem_count};
  std::cout << "main_bench - len(" << array_length << ") - witem_count(" << witem_count << ")\n";

  for (std::size_t r = 0; r < repeat_count; ++r)
  {
    bench::chrono_t chrono; chrono.Init();

    double* in_array  = new double[array_length];
    for (std::size_t i = 0; i < array_length; ++i) { in_array[i]  = i; }

    auto in_view = kwk::view{kwk::source = in_array , kwk::of_size(array_length)};
    timer(ritem.host_alloc, chrono);
    double res_device = ::kwk::reduce(ctx, in_view, global_fct, double{0});
    timer(ritem.copies_and_kernel, chrono);

    double res_verif = 0;
    for (std::size_t i = 0; i < array_length; ++i) { res_verif = global_fct(res_verif, in_array[i]); }
    std::cout << "    main_bench len(" << array_length << ") res_device(" << res_device << ") res_verif(" << res_verif << ")\n";
    check_tolerance(res_verif, res_device);
    timer(ritem.check, chrono);
    delete[] in_array;
  }
  return ritem;
};


bench::result_item main_bench_cpu_native(std::size_t witem_count, std::size_t repeat_count)
{
  std::size_t array_length = ARRAY_LENGTH;
  bench::result_item ritem{witem_count};
  std::cout << "main_bench_cpu_native - len(" << array_length << ") - witem_count(" << witem_count << ")\n";

  for (std::size_t r = 0; r < repeat_count; ++r)
  {
    bench::chrono_t chrono; chrono.Init();

    double* in_array  = new double[array_length];
    for (std::size_t i = 0; i < array_length; ++i) { in_array[i]  = i; }

    timer(ritem.host_alloc, chrono);
    double res_device = 0;
    for (std::size_t i = 0; i < array_length; ++i) { res_device = global_fct(res_device, in_array[i]); }
    timer(ritem.copies_and_kernel, chrono);

    double res_verif = 0;
    for (std::size_t i = 0; i < array_length; ++i) { res_verif = global_fct(res_verif, in_array[i]); }
    check_tolerance(res_verif, res_device);
    timer(ritem.check, chrono);
    delete[] in_array;
    // std::cout << "    main_bench_cpu_native - res_device(" << res_device << ") res_verif(" << res_verif << ")\n";
  }
  return ritem;
};



int main(int argc, char* argv[])
{
  std::string prefix = make_prefix();
  std::size_t file_version = 2;
  std::vector<std::size_t> workitem_size_vect;
  std::size_t val = 10;
  // for (std::size_t i = 0; i < INCREASE_SIZE_COUNT; ++i)
  // {
  //   workitem_size_vect.push_back(val);
  //   val *= 2;
  // }
  workitem_size_vect.push_back(ARRAY_LENGTH/2);

  array_printer_t ap;
  bench::result_vector rvect;
  ap.add({"device", "alloc_host", "cpy+ker", "check"});

  // sycl_context_cpu when compiled with "-fsycl"
  // sycl_context_gpu when compiled with "-fsycl -fsycl-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda"
  write_f.open(prefix + "reduce_sycl_context_cpu" + ".txt");
  write_f << file_version << "\n";
  rvect.clear();
  ap.add({"SYCL", "-", "-", "-"});
  for (std::size_t size : workitem_size_vect)
  {
    kwk::sycl::default_context.set_workitem_count(size);
    ap.add({"size", std::to_string(size), "-", "-"});
    bench::result_item r = main_bench(kwk::sycl::default_context, size, REPEAT_ITERATION_COUNT);
    rvect.items.push_back(r);
    for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
    {
      ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
    }
  }
  rvect.write_to_gfile();
  write_f.close();



  // write_f.open(prefix + "reduce_cpu_context.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"CPU", "-", "-", "-"});
  // for (std::size_t size : workitem_size_vect)
  // {
  //   ap.add({"size", std::to_string(size), "-", "-"});
  //   bench::result_item r = main_bench(kwk::cpu, size, REPEAT_ITERATION_COUNT);
  //   rvect.items.push_back(r);
  //   for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
  //   {
  //     ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
  //   }
  // }
  // rvect.write_to_gfile();
  // write_f.close();

  

  // write_f.open(prefix + "reduce_cpu_native.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"CPU-NATIVE", "-", "-", "-"});
  // for (std::size_t size : workitem_size_vect)
  // {
  //   ap.add({"size", std::to_string(size), "-", "-"});
  //   bench::result_item r = main_bench_cpu_native(size, REPEAT_ITERATION_COUNT);
  //   rvect.items.push_back(r);
  //   for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
  //   {
  //     ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
  //   }
  // }
  // rvect.write_to_gfile();
  // write_f.close();

  ap.print();
}

// On fait donc :
// test/algorithm/algos/<les algos.cpp sans contexte>
// et des test/algorithm/algos/cpu/  sycl/ etc
