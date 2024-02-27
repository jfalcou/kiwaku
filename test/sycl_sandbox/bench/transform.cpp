//======================================================================================================================
/*
  KIWAKU - Containers Well Made
  Copyright : KIWAKU Contributors & Maintainers
  SPDX-License-Identifier: BSL-1.0
*/
//======================================================================================================================

// icpx -fsycl proxy_poc_axpy_bench.cpp -o e -O3 -std=c++20 -I/home/data_sync/academique/These/kiwaku_2023/kiwaku/include && ./e

// Exécution GPU SYCL :
// icpx -fsycl  -fsycl-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda transform.cpp -o e -O3 -std=c++20 -I/mnt/chaton/kiwaku/include && ./e
// LEGEND:
// icpx -fsycl  -fsycl-targets=nvptx64-nvidia-cuda --cuda-path=/opt/cuda transform.cpp -o e -O3 -std=c++20 -I/home/sylvainj/sshmount/kiwaku/include && ./e

// Exécution CPU SYCL :
// icpx -fsycl transform.cpp -o e -O3 -std=c++20 -I/mnt/chaton/kiwaku/include && ./e
// Sur LEGEND: 
// icpx -fsycl transform.cpp -o e -O3 -std=c++20 -I/home/sylvainj/sshmount/kiwaku/include && ./e


// LEGEND: Run both x86_64 cpu and Nvidia GPU
// icpx -fsycl  -fsycl-targets=nvptx64-nvidia-cuda,x86_64 --cuda-path=/opt/cuda transform.cpp -o e -O3 -std=c++20 -I/home/sylvainj/sshmount/kiwaku/include && ./e



#include <kwk/algorithm/algos/transform.hpp>
#include <kwk/container.hpp>
#include <cmath>

#include <kwk/context/sycl/context.hpp>
#include <kwk/context/cpu/context.hpp>
#include "../utils/utils.hpp"

#define HEAVY true
#define INCREASE_SIZE_COUNT 1
// #define INCREASE_SIZE_COUNT 6
// #define INCREASE_SIZE_COUNT 1
#define REPEAT_ITERATION_COUNT 7

// bool G_HEAVY = false;

std::string make_prefix()
{
  return "transform4_"
         + get_computer_name() + "_" + (HEAVY?"heavy":"copy")
         + "_isc" + std::to_string(INCREASE_SIZE_COUNT) 
         + "_rp" + std::to_string(REPEAT_ITERATION_COUNT)
        //  + "_ndr" + std::to_string(ND_RANGE_LOCAL)
         + "_";
}

// auto get_function()
// {
//   if (G_HEAVY)
//   {
//     auto fct = [](auto x)
//     {
//       return 1 / ((1 - std::atan(x))
//               + (1
//                 / (2
//                   + std::atan( 5 / (3 + std::atan(std::atan(std::atan(std::atan(x)))))))));
//     };
//     return fct;
//   }
//   else 
//   {
//     return [](auto e) { return e; };
//   }
// }

#if HEAVY
  // std::string make_suffix() {
  //   return std::string{"_heavy_"} + get_computer_name();
  // }

  auto global_fct = [](auto x)
  {
    return 1 / ((1 - std::atan(x))
            + (1
              / (2
                + std::atan( 5 / (3 + std::atan(std::atan(std::atan(std::atan(x)))))))));
  };
#else
  // std::string make_suffix() {
  //   return std::string{"_copy_"} + get_computer_name();
  // }
  auto global_fct = [](auto e) { return e; }; // 1.0/(1.0+e)
#endif


namespace bench
{
  struct result_item
  {
    result_item(std::size_t array_size_) : array_size(array_size_) {}
    std::size_t array_size;
    // std::size_t nd_range_size;
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
        write_f << item.array_size << "\n";
        // write_f << item.nd_range_size << "\n";
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



template<typename Context>
bench::result_item main_bench(Context& ctx, std::size_t array_length, std::size_t repeat_count)
{
  // Étape 1 :
  // faire un bench de transform sur cpu avec en x la taille du tableau et en y le temps pris.
  // - 

  bench::result_item ritem{array_length};
  // ritem.nd_range_size = ND_RANGE_LOCAL;

  // std::cout << "main_bench - array_length(" << array_length << ") - nd_range_size(" << ND_RANGE_LOCAL << ")\n";
  std::cout << "main_bench - array_length(" << array_length << ")\n";

  const double ERROR_TOLERANCE = 0.001;

  for (std::size_t r = 0; r < repeat_count; ++r)
  {
    bench::chrono_t chrono;
    chrono.Init();
    double* in_array  = new double[array_length];
    double* out_array = new double[array_length];

    for (std::size_t i = 0; i < array_length; ++i) { in_array[i]  = i; out_array[i] = 0; }

    auto in_view  = kwk::view{kwk::source = in_array , kwk::of_size(array_length)};
    auto out_view = kwk::view{kwk::source = out_array, kwk::of_size(array_length)};


    ritem.host_alloc.push_back(chrono.ElapsedTimeMSReset());

    ::kwk::transform(ctx, global_fct, out_view, in_view);

    ritem.copies_and_kernel.push_back(chrono.ElapsedTimeMSReset());

    for (std::size_t i = 0; i < array_length; ++i)
    {
      assert(std::abs(out_array[i] - global_fct(in_array[i])) < ERROR_TOLERANCE);
    }

    ritem.check.push_back(chrono.ElapsedTimeMSReset());
    delete[] in_array;
    delete[] out_array;
  }

  return ritem;
};

bench::result_item main_bench_cpu_naive(std::size_t array_length, std::size_t repeat_count)
{
  bench::result_item ritem{array_length};

  std::cout << "main_bench - len(" << array_length << ")\n";

  const double ERROR_TOLERANCE = 0.001;

  for (std::size_t r = 0; r < repeat_count; ++r)
  {
    bench::chrono_t chrono;
    chrono.Init();
    double* in_array  = new double[array_length];
    double* out_array = new double[array_length];

    for (std::size_t i = 0; i < array_length; ++i) { in_array[i]  = i; out_array[i] = 0; }

    ritem.host_alloc.push_back(chrono.ElapsedTimeMSReset());

    for (std::size_t i = 0; i < array_length; ++i)
    {
      out_array[i] = global_fct(in_array[i]);
    }

    ritem.copies_and_kernel.push_back(chrono.ElapsedTimeMSReset());

    for (std::size_t i = 0; i < array_length; ++i)
    {
      assert(std::abs(out_array[i] - global_fct(in_array[i])) < ERROR_TOLERANCE);
    }

    ritem.check.push_back(chrono.ElapsedTimeMSReset());
    delete[] in_array;
    delete[] out_array;
  }

  return ritem;
};



int main(int argc, char* argv[])
{
  std::string prefix = make_prefix();
  std::size_t file_version = 2;
  std::vector<std::size_t> array_size_vect;
  std::size_t val = 12800000 * 128; // 32 -> 128
  for (std::size_t i = 0; i < INCREASE_SIZE_COUNT; ++i)
  {
    array_size_vect.push_back(val);
    val *= 2;
  }

  // std::vector<std::size_t> nd_range_local_size;
  // for (std::size_t val = 8; val <= 1024; val *= 2)
  // {
  //   nd_range_local_size.push_back(val);
  // }

  std::size_t array_size = array_size_vect[0];

  array_printer_t ap;
  bench::result_vector rvect;
  ap.add({"device", "alloc_host", "cpy+ker", "check"});

  kwk::sycl::context sycl_gpu_ctx{::sycl::gpu_selector_v};
  kwk::sycl::context sycl_cpu_ctx{::sycl::cpu_selector_v};

  // // sycl_context_cpu when compiled with "-fsycl"
  // // sycl_context_gpu when compiled with "-fsycl -fsycl-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda"
  // write_f.open(prefix + "sycl_gpu_ndr.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"SYCL", "-", "-", "-"});
  // for (std::size_t ndr_size : nd_range_local_size)
  // {
  //   ND_RANGE_LOCAL = ndr_size;
  //   ap.add({"array_size", std::to_string(array_size), "-", "-"});
  //   ap.add({"ndr_size", std::to_string(ndr_size), "-", "-"});
  //   bench::result_item r = main_bench(sycl_gpu_ctx, array_size, REPEAT_ITERATION_COUNT); // kwk::sycl::default_context
  //   rvect.items.push_back(r);
  //   for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
  //   {
  //     ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
  //   }
  // }
  // rvect.write_to_gfile();
  // write_f.close();

  // write_f.open(prefix + "sycl_cpu_ndr.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"SYCL", "-", "-", "-"});
  // for (std::size_t ndr_size : nd_range_local_size)
  // {
  //   ND_RANGE_LOCAL = ndr_size;
  //   ap.add({"array_size", std::to_string(array_size), "-", "-"});
  //   ap.add({"ndr_size", std::to_string(ndr_size), "-", "-"});
  //   bench::result_item r = main_bench(sycl_cpu_ctx, array_size, REPEAT_ITERATION_COUNT);
  //   rvect.items.push_back(r);
  //   for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
  //   {
  //     ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
  //   }
  // }
  // rvect.write_to_gfile();
  // write_f.close();

  // write_f.open(prefix + "cpu_context_ndr.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"CPU", "-", "-", "-"});
  // for (std::size_t ndr_size : nd_range_local_size)
  // {
  //   ND_RANGE_LOCAL = ndr_size;
  //   ap.add({"array_size", std::to_string(array_size), "-", "-"});
  //   ap.add({"ndr_size", std::to_string(ndr_size), "-", "-"});
  //   bench::result_item r = main_bench(kwk::cpu, array_size, REPEAT_ITERATION_COUNT);
  //   rvect.items.push_back(r);
  //   for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
  //   {
  //     ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
  //   }
  // }
  // rvect.write_to_gfile();
  // write_f.close();



  // sycl_context_cpu when compiled with "-fsycl"
  // sycl_context_gpu when compiled with "-fsycl -fsycl-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda"
  // write_f.open(prefix + "sycl_gpu.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"SYCL", "-", "-", "-"});
  // for (std::size_t size : array_size_vect)
  // {
  //   ap.add({"size", std::to_string(size), "-", "-"});
  //   bench::result_item r = main_bench(sycl_gpu_ctx, size, REPEAT_ITERATION_COUNT); // kwk::sycl::default_context
  //   rvect.items.push_back(r);
  //   for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
  //   {
  //     ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
  //   }
  // }
  // rvect.write_to_gfile();
  // write_f.close();


  write_f.open(prefix + "sycl_cpu4.txt");
  write_f << file_version << "\n";
  rvect.clear();
  ap.add({"SYCL", "-", "-", "-"});
  for (std::size_t size : array_size_vect)
  {
    ap.add({"size", std::to_string(size), "-", "-"});
    bench::result_item r = main_bench(sycl_cpu_ctx, size, REPEAT_ITERATION_COUNT);
    rvect.items.push_back(r);
    for (std::size_t i = 0; i < REPEAT_ITERATION_COUNT; ++i)
    {
      ap.add({"", std::to_string(r.host_alloc[i]), std::to_string(r.copies_and_kernel[i]), std::to_string(r.check[i])});
    }
  }
  rvect.write_to_gfile();
  write_f.close();

  // write_f.open(prefix + "cpu_context.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"CPU", "-", "-", "-"});
  // for (std::size_t size : array_size_vect)
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






  // write_f.open(prefix + "cpu_native.txt");
  // write_f << file_version << "\n";
  // rvect.clear();
  // ap.add({"CPU-NATIVE", "-", "-", "-"});
  // for (std::size_t size : array_size_vect)
  // {
  //   ap.add({"size", std::to_string(size), "-", "-"});
  //   bench::result_item r = main_bench_cpu_naive(size, REPEAT_ITERATION_COUNT);
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
